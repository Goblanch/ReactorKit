#include "Conditions/RKDistanceCondition.h"

DEFINE_LOG_CATEGORY_STATIC(LogRKDistanceCondition, Log, All);

bool URKDistanceCondition::Evaluate_Implementation(const FRKEventPayload& Payload) const
{
	if (!Payload.Instigator)
	{
		UE_LOG(LogRKDistanceCondition, Warning,
			TEXT("[RKDistanceCondition] Instigator is null — returning false."));
		return false;
	}
	
	if (!Payload.Target)
	{
		UE_LOG(LogRKDistanceCondition, Warning,
			TEXT("[RKDistanceCondition] Target is null — returning false."));
		return false;
	}
	
	if (MinDistance > MaxDistance)
	{
		UE_LOG(LogRKDistanceCondition, Warning,
			TEXT("[RKDistanceCondition] MinDistance (%.1f) is greater than MaxDistance (%.1f) — returning false."),
			MinDistance, MaxDistance);
		return false;
	}
	
	const FVector InstigatorLocation = Payload.Instigator->GetActorLocation();
	const FVector TargetLocation = Payload.Target->GetActorLocation();
	const float Distance = FVector::Dist(InstigatorLocation, TargetLocation);
	
	const bool Result = (Distance >= MinDistance && Distance <= MaxDistance);
	
	UE_LOG(LogRKDistanceCondition, Log,
		TEXT("[RKDistanceCondition] Distance between '%s' and '%s': %.1f cm — range: [%.1f, %.1f] — result: %s"),
		*Payload.Instigator->GetName(),
		*Payload.Target->GetName(),
		Distance,
		MinDistance,
		MaxDistance,
		Result ? TEXT("PASS") : TEXT("FAIL"));

	return Result;
}
