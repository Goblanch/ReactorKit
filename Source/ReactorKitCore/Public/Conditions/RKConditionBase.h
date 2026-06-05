#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Triggers/RKEventPayload.h"
#include "RKConditionBase.generated.h"

/**
 * Abstract base class for all conditions in the ReactorKit ECA system.
 * A condition evaluates a specific rule against an FRKEventPayload and
 * returns true if the rule passes or false if it does not.
 * 
 * Conditions are evaluated by URKConditionTree before the ECA Chain executes.
 * Multiple conditions can be combined using AND, OR and NOT operators
 * inside URKConditionTree.
 * 
 * Extend this class in C++ or Blueprint to implement custom evaluation logic:
 * 
 * C++ Subclass:
 * @code 
*   virtual bool Evaluate_Implementation(const FRKEventPayload& Payload) override
 *  {
 *       return Payload.Instigator != nullptr;
 *  }
 * @endcode
 * 
 * Blueprint subclass:
 *   Create a Blueprint child of URKConditionBase and override the Evaluate event.
 *   Return true or false from the event graph.
 *   
 * This class is Abstract and cannot be instantiated directly - it must be subclassed.
 */
UCLASS(Blueprintable, Abstract, EditInlineNew)
class REACTORKITCORE_API URKConditionBase : public UObject
{
	GENERATED_BODY()
	
public:
	/**
	 * Evaluates this condition against the given event payload.
	 * Returns true if the condition passes, false if it does not.
	 * 
	 * The base implementation always returns true - subclasses must override
	 * this method to provide meaningful evaluation logic.
	 * 
	 * Override Evaluate_Implementation() in C++ subclasses.
	 * Override Evaluate() directly in Blueprint subclasses.
	 * 
	 * @param Payload The event data to evaluate against.
	 * @return True if the condition passes, false otherwise.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="ReactorKit|Condition")
	bool Evaluate(const FRKEventPayload& Payload) const;
	virtual bool Evaluate_Implementation(const FRKEventPayload& Payload) const;
};