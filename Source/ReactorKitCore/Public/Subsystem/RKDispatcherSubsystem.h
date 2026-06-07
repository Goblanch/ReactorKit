#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Triggers/RKEventPayload.h"
#include "RKDispatcherSubsystem.generated.h"

// -- Delegate declarations --------------------------------------------------------------------------------------------

/**
 * Internal multicast delegate used by the DispatcherSubsystem registry.
 * Used exclusively from C++ — Blueprint listeners use OnEventDispatched instead.
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FRKEventDelegate, const FRKEventPayload&);

/**
 * Dynamic multicast delegate exposed to Blueprint.
 * Blueprint actors bind to this delegate via the OnEventDispatched property
 * to receive events dispatched through the subsystem.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRKEventDelegateDynamic, const FRKEventPayload&, Payload);

// -- Subsystem --------------------------------------------------------------------------------------------------------

/**
 * Central event bus for the ReactorKit ECA system.
 * Implemented as a GameInstanceSubsystem so it persists for the entire 
 * game session and is accesible from any context without a direct reference.
 * 
 * Triggers dispatch events by name. Any actor or object can register as a 
 * listener for any event name without knowing who fires it.
 * 
 * Access from Blueprint:
 *	GetGameInstance -> Get Subsystem (URKDispatcherSubsystem)
 *	
 * Access from C++:
 * @code 
 *	UGameInstance* GI = GetGameInstance();
 *	URKDispatcherSubsystem* Dispatcher = GI->GetSubsystem<URKDisptcherSubsystem>();
 * @endcode 
 */
UCLASS()
class REACTORKITCORE_API URKDispatcherSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	
	// -- UGameInstanceSubsystem interface -----------------------------------------------------------------------------

	/**
	 * Called when the subsystem is initialized with the GameInstance.
	 * Use this to set up any initial state.
	 */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/**
	 * Called when the subsystem is about to be destroyed.
	 * Clears the listener registry to prevent stale delegate references.
	 */
	virtual void Deinitialize() override;
	
	// -- Public API ---------------------------------------------------------------------------------------------------

	/**
	 * Registers a C++ delegate as listener for the given event name.
	 * Multiple delegates can be registered for the same event name.
	 * Returns a FDelegateHandle that must be stored and passed to
	 * UnregisterListener() to remove listener later.
	 * 
	 * @param EventName Name of the event to listen for.
	 * @param Delegate Delegate to call when the event is dispatched.
	 * @return Handle to use when unregistering this listener.
	 */
	FDelegateHandle RegisterListener(FName EventName, const FRKEventDelegate::FDelegate& Delegate);

	/**
	 * Unregisters a previously registered C++ delegate from the given event name.
	 * Safe to call with an invalid handle or unknown event name - no crash, no side effects.
	 *
	 * NOTE: Stub implementation — logs the call only. Full logic in Issue #10.
	 *
	 * @param EventName     Name of the event to stop listening for.
	 * @param Handle        The handle returned by RegisterListener.
	 */
	void UnregisterListener(FName EventName, FDelegateHandle Handle);

	/**
	 * Dispatchers an event by name, notifying all registered listeners with the payload.
	 * If no listeners are registered for the event name, the call is a no-op.
	 * 
	 * @param EventName Name of the event to dispatch.
	 * @param Payload Event data to pass to all listeners.
	 */
	UFUNCTION(BlueprintCallable, Category="ReactorKit|Dispatcher")
	void DispatchEvent(FName EventName, const FRKEventPayload& Payload);
	
	/**
	 * Dynamic multicast delegate exposed to Blueprint.
	 * Blueprint actors bind to this to receive any event dispatched
	 * through the subsystem, then filter by EventName in the payload.
	 *
	 * In C++, use RegisterListener() per event name for more granular control.
	 */
	UPROPERTY(BlueprintAssignable, Category = "ReactorKit|Dispatcher")
	FRKEventDelegateDynamic OnEventDispatched;
	
private:
	/**
	 * Internal mapo from event name to its multicast delegate.
	 * Each entry holds all listeners registered for that event name.
	 * Populated lazily - entries are created on first RegisterListener call.
	 */
	TMap<FName, FRKEventDelegate> ListenerRegistry;
};