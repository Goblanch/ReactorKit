#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Triggers/RKEventPayload.h"
#include "RKActionBase.generated.h"

/**
 * Abstract base class for all actions in the ReactorKit ECA system.
 * An action executes a specific piece of game logic in response to a
 * validated event - after the Trigger has fired and the ConditionTree
 * has returned true.
 * 
 * Actions are executed sequentially by URKECAChain. Each action can define 
 * a DelayBeforeExecution to introduce a pause before its logic runs.
 * 
 * Extend this class in C++ or Blueprint to implement custom action logic:
 * @code
 *   virtual void Execute_Implementation(const FRKEventPayload& Payload) override
 *   {
 *       // your logic here
 *   }
 * @endcode
 * 
 * Blueprint subclass:
 *	Create a Blueprint child of URKActionBase and override the Execute event.
 *	Implement your logic in the event graph.
 *	
 * @note This class is Abstract and cannot be instantiated directly - it must be subclassed.
 */
UCLASS(Blueprintable, Abstract, EditInlineNew)
class REACTORKITCORE_API URKActionBase : public UObject
{
	GENERATED_BODY()
	
public:
	
	// -- Configuration ------------------------------------------------------------------------------------------------

	/**
	 * Time in seconds to wait before this action executes.
	 * Used by URKECAChain to introduce a pause between actions in a sequence.
	 * Set to 0 to execute immediately after the previous action completes.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ReactorKit|Action",
		meta=(ClampMin = "0.0", UIMin = "0.0"))
	float DelayBeforeExecution = 0.0f;
	
	// -- Public API ---------------------------------------------------------------------------------------------------

	/**
	 * Executes this action with the given event payload.
	 * Called by URKECAChain after any DelayBeforeExecution has elapsed.
	 * 
	 * The base implementation does nothing and logs a warning - subclasses
	 * must override this method to provide meaningful action logic.
	 * 
	 * Override Execute_Implementation() in C++ subclasses.
	 * Override Execute() directly in Blueprint subclasses.
	 * 
	 * @param Payload The event data from the trigger that started this chain.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="ReactorKit|Action")
	void Execute(const FRKEventPayload& Payload);
	virtual void Execute_Implementation(const FRKEventPayload& Payload);
};