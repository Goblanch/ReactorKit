#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Triggers/RKEventPayload.h"
#include "Actions/RKActionBase.h"
#include "RKECAChain.generated.h"

/**
 * Executes a sequential chain of URKActionBase instances in order,
 * respecting each action's DelayBeforeExecution property.
 * 
 * The chain is owned by an ARKReactorActor and executes when a Trigger
 * fires and the ConditionTree passes. Actions run one after another,
 * each waiting for its configured delay before executing.
 * 
 * Concurrent executions are prevented by bIsExecuting - calling Execute()
 * while the chain is already running is a no-op with a log warning.
 * Call Interrupt() first to cancel a running chain before re-executing.
 * 
 * Timer management:
 *		Each delayed action uses a dedicated FTimerHandle stored in PendingTimers.
 *		Interrupt() clears all pending timers cleanly to prevent leaks.
 *		The chain requires a valid UWorld to manage timers - ensure the owning
 *		actor is in a world before calling Execute().
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class REACTORKITCORE_API URKECAChain : public UObject
{
	GENERATED_BODY()
	
public:
	
	// -- Configuration ------------------------------------------------------------------------------------------------

	/**
	 * Ordered list of actions to execute in sequence.
	 * Each action runs after its DelayBeforeExecution has elapsed.
	 * Null entries are skipped with a warning.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = "ReactorKit|ECAChain")
	TArray<TObjectPtr<URKActionBase>> Actions;
	
	// -- Public API ---------------------------------------------------------------------------------------------------
	
	/**
	 * Begins a sequential execution of all actions in the chain.
	 * Actions are executed in array order, each after its DelayBeforeExecution.
	 * 
	 * No-op if the chain is already executing - call Interrupt() first
	 * to cancel the current execution before starting a new one.
	 * 
	 * Requires a valid outer UObject with a reachable UWorld for timer management.
	 * 
	 * @param Payload Event data passed to each action's Execute() call.
	 */
	UFUNCTION(BlueprintCallable, Category = "ReactorKit|ECAChain")
	void Execute(const FRKEventPayload& Payload);
	
	/**
	 * Interrupts the current chain execution and clears all pending timers.
	 * Safe to call at any time - no-op if the chain is not executing.
	 * After Interrupt(), Execute() can be called again immediately.
	 */
	UFUNCTION(BlueprintCallable, Category = "ReactorKit|ECAChain")
	void Interrupt();
	
	/**
	 * Returns true if the chain is currently executing.
	 */
	UFUNCTION(BlueprintPure, Category = "ReactorKit|ECAChain")
	bool IsExecuting() const { return bIsExecuting; }

	/**
	 * Creates a new URKECAChain instance.
	 * Use this node in Blueprint to create a chain at runtime.
	 * 
	 * @param Outer The object that owns, typically the Reactor Actor.
	 * @return A new URKECAChain instance ready to configure.
	 */
	UFUNCTION(BlueprintCallable, Category = "ReactorKit|ECAChain")
	static URKECAChain* CreateECAChain(UObject* Outer);
	
private:
	
	// -- Internal State -----------------------------------------------------------------------------------------------

	/**
	 * Guard flag - true while the chain is executing.
	 * Prevents concurrent executions of the same chain.
	 */
	bool bIsExecuting = false;
	
	/**
	 * Stores the payload for the current execution so timer callbacks
	 * can access it without needing to pass it through the timer system.
	 */
	FRKEventPayload ActivePayload;
	
	/**
	 * List of timer handles for all pending delayed actions.
	 * Populated during Execute() and cleared by Interrupt().
	 */
	TArray<FTimerHandle> PendingTimers;

	/**
	 * Executes a single action at the given index.
	 * Called directly for zero-delay actions or via timer for delayed ones.
	 * 
	 * @param ActionIndex Index into the Actions array.
	 */
	void ExecuteActionAtIndex(int32 ActionIndex);
	
	/**
	 * Clears all pending timer handles from the world's timer manager.
	 * Called by Interrupt() and at end of chain execution.
	 */
	void ClearAllTimers();
	
	/**
	 * Returns the UWorld from the outer object chain.
	 * Required for timer management.
	 */
	UWorld* GetWorld() const override;
};