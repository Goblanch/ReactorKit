#include "Conditions/RKTagCondition.h"
#include "GameplayTagAssetInterface.h"

DEFINE_LOG_CATEGORY_STATIC(LogRKTagCondition, Log, All);

bool URKTagCondition::Evaluate_Implementation(const FRKEventPayload& Payload) const
{
	if (!Payload.Instigator)
	{
		UE_LOG(LogRKTagCondition, Warning,
			TEXT("[RKTagCondition] Instigator is null — returning false."));
		return false;
	}
	
	if (VariableName.IsNone())
	{
		UE_LOG(LogRKTagCondition, Warning,
			TEXT("[RKTagCondition] VariableName is empty — returning false."));
		return false;
	}
	
	if (!RequiredTag.IsValid())
	{
		UE_LOG(LogRKTagCondition, Warning,
			TEXT("[RKTagCondition] RequiredTag is not valid — returning false."));
		return false;
	}
	
	const UClass* InstigatorClass = Payload.Instigator->GetClass();
	bool bHasTag = false;
	
	if (const FStructProperty* StructProp = CastField<FStructProperty>(
		InstigatorClass->FindPropertyByName(VariableName)))
	{
		if (StructProp->Struct == FGameplayTag::StaticStruct())
		{
			const FGameplayTag* TagValue = StructProp->ContainerPtrToValuePtr<FGameplayTag>(Payload.Instigator);
			bHasTag = TagValue && (*TagValue == RequiredTag);
		}else if (StructProp->Struct == FGameplayTagContainer::StaticStruct())
		{
			const FGameplayTagContainer* Container = StructProp->ContainerPtrToValuePtr<FGameplayTagContainer>(Payload.Instigator);
			bHasTag = Container && Container->HasTag(RequiredTag);
		}else
		{
			UE_LOG(LogRKTagCondition, Warning,
				TEXT("[RKTagCondition] Property '%s' on '%s' is not a FGameplayTag or FGameplayTagContainer — returning false."),
				*VariableName.ToString(),
				*InstigatorClass->GetName());
			return false;
		}
	}else
	{
		UE_LOG(LogRKTagCondition, Warning,
			TEXT("[RKTagCondition] Property '%s' not found on '%s' — returning false."),
			*VariableName.ToString(),
			*InstigatorClass->GetName());
		return false;
	}
	
	const bool Result = (bHasTag == bMustHaveTag);
	
	UE_LOG(LogRKTagCondition, Log,
		TEXT("[RKTagCondition] Tag '%s' in '%s' on '%s' — hasTag: %s — mustHave: %s — result: %s"),
		*RequiredTag.ToString(),
		*VariableName.ToString(),
		*InstigatorClass->GetName(),
		bHasTag ? TEXT("true") : TEXT("false"),
		bMustHaveTag ? TEXT("true") : TEXT("false"),
		Result ? TEXT("PASS") : TEXT("FAIL"));

	return Result;
}
