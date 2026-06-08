#pragma once

#include "CoreMinimal.h"
#include "Conditions/RKConditionBase.h"
#include "RKDistanceCondition.generated.h"

/**
 * Predefined condition that evaluates the 3D world distance between
 * the payload Instigator and Target actors.
 * 
 * Returns true when the distance falls within [MinDistance, MaxDistance].
 * Useful for proximity checks.
 * 
 * Returns false and logs a warning when:
 *		- The Instigator is null.
 *		- The Target is null.
 */
UCLASS(Blueprintable, EditInlineNew, meta = (DisplayName = "Distance Condition"))
class REACTORKITCORE_API URKDistanceCondition : public URKConditionBase
{
	GENERATED_BODY()
	
public:
	
	/**
	 * Minimum distance in Unreal Units (cm) required to pass.
	 * Set to 0 to allow any distance up to MaxDistance.
	 * Must be less than or equal to MaxDistance.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ReactorKit|Condition|Distance",
		meta = (ClampMin = "0.0", UIMin = "0.0",
			ToolTip = "Minimum distance in cm between Instigator and Target. Default 0."))
	float MinDistance = 0.0f;
	
	/**
	 * Maximum distance in Unreal Engine Units (cm) allowed to pass.
	 * The condition fails if the distance exceeds this value.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ReactorKit|Condition|Distance",
		meta = (ClampMin = "0.0", UIMin = "0.0",
			ToolTip = "Maximum distance in cm between Instigator and Target."))
	float MaxDistance = 500.0f;

	/**
	 * Calculates the 3D world distance between the payload Instigator
	 * and Target and checks if it falls within [MinDistance, MaxDistance].
	 * 
	 * @param Payload Event data - both Instigator and Target must be valid.
	 * @return True if the distance is within the configured range.
	 */
	virtual bool Evaluate_Implementation(const FRKEventPayload& Payload) const override;
};