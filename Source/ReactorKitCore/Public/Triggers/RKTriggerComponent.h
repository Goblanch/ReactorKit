#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Triggers/RKEventPayload.h"
#include "RKTriggerComponent.generated.h"

/**
 * Base trigger component for the ReactorKit ECA system.
 * All predefined and custom triggers inherit from this class.
 * 
 * Subclass this in C++ or Blueprint and override Activate() to implement
 * custom trigger detection logic. The base implementation handles cooldown
 * enforcement, max activation limits, and enabled/disabled state.
 * 
 * Usage:
 *	- Add to any actor in the editor as a component.
 *	- Configure MaxActivation, Cooldown, and bEnabled from the details panel.
 *	- Call EnableTrigger() / DisableTrigger() at runtime from Blueprint or C++.
 */
UCLASS(Blueprintable, ClassGroup=ReactorKit, meta=(BlueprintSpawnableComponent))
class REACTORKITCORE_API URKTriggerComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	
	URKTriggerComponent();
	
	// -- Configuration ------------------------------------------------------------------------------------------------
	
	/**
	 * Whether this trigger is active and can fire events.
	 * When false, calls to Activate() are silently ignored.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ReactorKit|Trigger")
	bool bEnabled = true;
	
	/**
	 * Maximum number of times this trigger can fire.
	 * Set to 0 for unlimited activation.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ReactorKit|Trigger", meta=(ClampMin = "0"))
	int32 MaxActivations = 0;
	
	/**
	 * Minimum time in seconds that must pass between activations.
	 * Set to 0 to allow immediate re-activation.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ReactorKit|Trigger", meta=(ClampMin = "0.0"))
	float Cooldown = 0.0f;
	
	// -- Public API ---------------------------------------------------------------------------------------------------

	/**
	 * Main activation entry point for this trigger.
	 * The base implementation checks bEnabled, MacActivations, and Cooldown
	 * before allowing the activation to proceed.
	 * 
	 * Override FireTrigger_Implementation() in C++ subclasses or override 
	 * FireTrigger() in Blueprint subclasses to add custom detection logic.
	 * Always call Super::FireTrigger_Implementation() first when overriding in C++.
	 * 
	 * @param Payload Event data to propagate through the ECA system.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="ReactorKit|Trigger")
	void FireTrigger(const FRKEventPayload& Payload);
	virtual void FireTrigger_Implementation(const FRKEventPayload& Payload);

	/**
	 * Enables this trigger so it can fire events.
	 * Safe to call at any time from Blueprint or C++.
	 */
	UFUNCTION(BlueprintCallable, Category="ReactorKit|Trigger")
	void EnableTrigger();

	/**
	 * Disable this trigger so it no longer fires events.
	 * Any activation attempt while disabled is silently ignored.
	 * Safe to call at any time from Blueprint or C++.
	 */
	UFUNCTION(BlueprintCallable, Category="ReactorKit|Trigger")
	void DisableTrigger();

	/**
	 * Returns the number of times this trigger has successfully fired.
	 */
	UFUNCTION(BlueprintPure, Category="ReactorKit|Trigger")
	int32 GetActivationCount() const { return ActivationCount; }

	/**
	 * Resets the activation count and last activation time.
	 * Useful when reloading state or restarting a sequence.
	 */
	UFUNCTION(BlueprintCallable, Category="ReactorKit|Trigger")
	void ResetActivationState();
	
protected:
	// -- Initial state ------------------------------------------------------------------------------------------------

	/**
	 * Number of times this trigger has successfully fired.
	 * Compared against MaxActivation to enforce the activation limit.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ReactorKit|Trigger|State")
	int32 ActivationCount = 0;

	/**
	 * World time in seconds at which this trigger last fired.
	 * Used to enforce the Cooldown between activations.s
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ReactorKit|Trigger|State")
	float LastActivationTime = -1.0f;
	
private:
	
	/**
	 * Returns true if all activation conditions are met:
	 * trigger is enabled, max activations not exceeded, and cooldowns has elapsed.
	 */
	bool CanActivate() const;
};