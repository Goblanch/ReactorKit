#pragma once

#include "CoreMinimal.h"
#include "Conditions/RKConditionBase.h"
#include "RKBoolCondition.generated.h"

/**
 * Predefined condition that evaluates a boolean variable on the payload
 * Instigator using UE5 reflection. Returns true when the named variable
 * matches the configured ExpectedValue.
 * 
 * Configure VariableName to match the exact name of a bool UPROPERTY
 * on the Instigator actor. Configure ExpectedValue to the value it must
 * have for this condition to pass.
 * 
 * Returns false and logs a warning when:
 *		- The Instigator is null
 *		- The named variable does not exist on the Instigator
 *		- The named variable is not a bool property
 */
UCLASS(Blueprintable, EditInlineNew, meta = (DisplayName = "Bool Condition"))
class REACTORKITCORE_API URKBoolCondition : public URKConditionBase
{
	GENERATED_BODY()
	
public:
	
	/**
	 * The name of the bool variable to read from the Instigator.
	 * Must match exactly the UPROPERTY name on the Instigator actor class.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReactorKit|Condition|Bool",
		meta = (ToolTip = "Exact name of the bool UPROPERTY to read from the Instigator."))
	FName VariableName;
	
	/**
	 * The value the bool variable must have for this condition to return true.
	 * Set to true to require the variable to be true, false to require it to be false.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReactorKit|Condition|Bool",
		meta = (ToolTip = "The bool variable must equal this value for the condition to pass"))
	bool ExpectedValue = true;

	/**
	 * Reads the named bool variable from the payload Instigator via UE5 reflection
	 * and compares it against ExpectedValue.
	 * 
	 * @param Payload Event data - Instigator is the actor whose variable is read.
	 * @return True if the variable exists and matches ExpectedValue.
	 */
	virtual bool Evaluate_Implementation(const FRKEventPayload& Payload) const override;
};