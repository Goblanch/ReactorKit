#pragma once

#include "CoreMinimal.h"
#include "Conditions/RKConditionBase.h"
#include "GameplayTagContainer.h"
#include "RKTagCondition.generated.h"

/**
 * Predefined condition that evaluates the presence of a Gameplay Tag
 * on the payload Instigator using the IGameplayTagAssetInterface.
 * 
 * Use bMustHaveTag to configure whether the condition passes when the 
 * actor HAS the tag or when it DOES NOT have the tag.
 * 
 * Returns false and logs warning when:
 *		- The Instigator is null.
 *		- The instigator does not implement IGameplayTagAssetInterface.
 */
UCLASS(Blueprintable, EditInlineNew, meta = (DisplayName = "Tag Condition"))
class REACTORKITCORE_API URKTagCondition : public URKConditionBase
{
	GENERATED_BODY()
	
public:
	/**
	 * The name of the FGameplayTag or FGameplayTagContainer variable
	 to read from the Instigator via reflection.
	 Must match exactly the UPROPERTY name on the Instigator actor class.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReactorKit|Condition|Tag",
		meta = (ToolTip = "Exact name of the FGameplayTag or FGameplayTagContainer UPROPERTY on the Instigator."))
	FName VariableName;
	
	/**
	 * The tag to check for in the variable.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReactorKit|Condition|Tag",
		meta = (ToolTip = "The Gameplay Tag to look for in the variable."))
	FGameplayTag RequiredTag;
	
	/**
	 * True - condition passes when the variable contains RequiredTag.
	 * False - condition passes when the variable does NOT contain RequiredTag.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReactorKit|Condition|Tag",
		meta = (ToolTip = "True: variable must contain the tag. False: variable must NOT contain the tag."))
	bool bMustHaveTag = true;
	
	/**
	 * Checks whether the payload Instigator has or does not have RequiredTag,
	 * depending on the bMustHaveTag configuration.
	 * 
	 * @param Payload Event data - Instigator is the actor whose tags are checked.
	 * @return True if the tag condition passes, false otherwise.
	 */
	virtual bool Evaluate_Implementation(const FRKEventPayload& Payload) const override;
};