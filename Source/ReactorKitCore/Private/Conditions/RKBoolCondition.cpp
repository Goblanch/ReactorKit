#include "Conditions/RKBoolCondition.h"

DEFINE_LOG_CATEGORY_STATIC(LogRKBoolCondition, Log, All);

bool URKBoolCondition::Evaluate_Implementation(const FRKEventPayload& Payload) const
{
	if (!Payload.Instigator)
	{
		UE_LOG(LogRKBoolCondition, Warning,
			TEXT("[RKBoolCondition] Instigator is null — returning false."));
		return false;
	}
	
	if (VariableName.IsNone())
	{
		UE_LOG(LogRKBoolCondition, Warning,
			TEXT("[RKBoolCondition] VariableName is empty — returning false."));
		return false;
	}
	
	// Find the property on the Instigator class via UE5 reflection
	const UClass* InstigatorClass = Payload.Instigator->GetClass();
	const FBoolProperty* BoolProp = CastField<FBoolProperty>(
		InstigatorClass->FindPropertyByName(VariableName));
	
	if (!BoolProp)
	{
		UE_LOG(LogRKBoolCondition, Warning,
			TEXT("[RKBoolCondition] Bool property '%s' not found on '%s' — returning false."),
			*VariableName.ToString(),
			*Payload.Instigator->GetClass()->GetName());
		return false;
	}
	
	const bool ActualValue = BoolProp->GetPropertyValue_InContainer(Payload.Instigator);
	bool Result = ActualValue == ExpectedValue;
	
	UE_LOG(LogRKBoolCondition, Log,
		TEXT("[RKBoolCondition] '%s' on '%s' — actual: %s — expected: %s — result: %s"),
		*VariableName.ToString(),
		*Payload.Instigator->GetClass()->GetName(),
		ActualValue ? TEXT("true") : TEXT("false"),
		ExpectedValue ? TEXT("true") : TEXT("false"),
		Result ? TEXT("PASS") : TEXT("FAIL"));

	return Result;
}
