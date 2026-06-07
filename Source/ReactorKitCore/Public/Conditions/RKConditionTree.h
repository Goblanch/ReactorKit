#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Triggers/RKEventPayload.h"
#include "Conditions/RKConditionBase.h"
#include "RKConditionTree.generated.h"

/**
 * Logical operator applied by URKConditionTree to combine
 * the results of all conditions in its Condition array.
 * 
 * AND — all conditions must return true
 * OR  — at least one condition must return true
 * NOT — inverts the result of the first condition in the array
 */
UENUM(BlueprintType)
enum class ERKConditionOperator : uint8
{
	AND		UMETA(DisplayName = "AND - All must pass"),
	OR		UMETA(DisplayName = "OR - At least one must pass"),
	NOT		UMETA(DisplayName = "NOT - Invert first condition")
};

/**
 * Evaluates a set of URKConditionBase instances using a logical operator
 * (AND, OR, NOT) and returns a single boolean.
 * 
 * Used by ARKReactorActor to decide whether the ECA Chain should execute
 * after a Trigger fires. An empty Conditions array always returns true,
 * meaning no conditions = no restriction on execute.
 * 
 * Operator behavior:
 *   AND — returns true only if every condition returns true
 *   OR  — returns true if at least one condition returns true
 *   NOT — returns the inverse of the first condition in the array
 *         (remaining conditions are ignored)
 *         
 * Conditions can be a mix of C++ and Blueprint subclasses of URKConditionBase.
 * Each condition is created inline via EditInLineNew in the Details panel.
 * 
 * This class is designed as a separate object (not embedded directly in the actor)
 * to allow future reuse as a shared DataAsset across multiple Reactor Actor.
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class REACTORKITCORE_API URKConditionTree : public UObject
{
	GENERATED_BODY()
	
public:
	
	// -- Configuration ------------------------------------------------------------------------------------------------
	
	/**
	 * Logical operator used to combine the results of all conditions.
	 * Defaults to AND - all condition must pass for the tree to return true.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ReactorKit|ConditionTree")
	ERKConditionOperator RootOperator = ERKConditionOperator::AND;
	
	/**
	 * List of conditions to evaluate.
	 * Each condition is an inline instance of a URKConditionBase subclass.
	 * An empty array returns true regardless of the operator.
	 * 
	 * For NOT operator, only the first condition in the array is evaluated.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category="ReactorKit|ConditionTree")
	TArray<TObjectPtr<URKConditionBase>> Conditions;
	
	// -- Public API ---------------------------------------------------------------------------------------------------

	/**
	 * Evaluates all condition using the configured RootOperator.
	 * Returns true if the combined result passes, false otherwise.
	 * 
	 * Edge cases:
	 *   - Empty Conditions array always returns true
	 *   - Null condition entries are skipped with a warning
	 *   - NOT operator only evaluates the first condition
	 * 
	 * @param Payload The event data to pass to each condition.
	 * @return True if the tree evaluation passes, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category="ReactorKit|ConditionTree")
	bool Evaluate(const FRKEventPayload& Payload);
	
	UFUNCTION(BlueprintCallable, Category="ReactorKit|ConditionTree",
		meta=(DeterminesOutputType = "Outer"))
	static URKConditionTree* CreateConditionTree(UObject* Outer);
	
private:
	
	/** Evaluates all conditions with AND logic - All must return true. */
	bool EvaluateAND(const FRKEventPayload& Payload) const;
	
	/** Evaluates all condition with OR logic - at least one must return true. */
	bool EvaluateOR(const FRKEventPayload& Payload) const;
	
	/** Inverts the result of the first condition in the array. */
	bool EvaluateNOT(const FRKEventPayload& Payload) const;
};