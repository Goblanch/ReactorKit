#pragma once

#include "CoreMinimal.h"
#include "Conditions/RKConditionBase.h"
#include "RKCustomCondition.generated.h"

/**
 * Predefined condition that delegates evaluation entirely to Blueprint or C++.
 * This class has no default evaluation logic - it is designed excluseively
 * to be subclassed.
 * 
 * Use this when none of the predefined conditions (Bool, Tag, Distance)
 * cover your specefic use case and, you need fully custom evaluation logic.
 * 
 * Blueprint usage:
 *   Create a Blueprint child of URKCustomCondition.
 *   Override the Evaluate event in the event graph.
 *   Return true or false based on your custom logic.
 *   Add an instance of your Blueprint child to a URKConditionTree.
 *
 * C++ usage:
 * @code
 *   UCLASS()
 *   class UMyCondition : public URKCustomCondition
 *   {
 *       GENERATED_BODY()
 *   public:
 *       virtual bool Evaluate_Implementation(const FRKEventPayload& Payload) const override
 *       {
 *           // your logic here
 *           return true;
 *       }
 *   };
 * @endcode
 * 
 * Unlike URKConditionBase, calling Evaluate() on an instance of this class
 * directly (without subclassing) will always return false and log an error,
 * makiung it inmediately obvious that subclassing is required.
 */
UCLASS(Blueprintable, EditInlineNew, meta = (DisplayName = "Custom Condition"))
class REACTORKITCORE_API URKCustomCondition : public URKConditionBase
{
	GENERATED_BODY()
	
public:
	/**
	 * Evaluation entry point - must be overriden in Blueprint or C++ subclasses.
	 * This implementation always returns false and logs an error to signal
	 * that URKCustomCondition must be subclassed before use.
	 * 
	 * Override this event in a Blueprint child to implement custom logic.
	 * Override Evaluate_Implementation() in a C++ child.
	 * 
	 * @param Payload The event data to evaluate against.
	 * @return False always - subclass must override to return meaningful value.
	 */
	virtual bool Evaluate_Implementation(const FRKEventPayload& Payload) const override;
};