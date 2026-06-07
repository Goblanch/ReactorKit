#include "Conditions/RKConditionTree.h"

DEFINE_LOG_CATEGORY_STATIC(LogRKConditionTree, Log, All);

bool URKConditionTree::Evaluate(const FRKEventPayload& Payload)
{
	if (Conditions.IsEmpty())
	{
		UE_LOG(LogRKConditionTree, Log,
			TEXT("[RKConditionTree] No conditions configured — returning true."));
		return true;
	}
	
	bool Result = false;
	
	switch (RootOperator)
	{
	case ERKConditionOperator::AND:
		Result = EvaluateAND(Payload);
		break;
	case ERKConditionOperator::OR:
		Result = EvaluateOR(Payload);
		break;
	case ERKConditionOperator::NOT:
		Result = EvaluateNOT(Payload);
		break;
	default:
		UE_LOG(LogRKConditionTree, Warning,
				TEXT("[RKConditionTree] Unknown operator — returning false."));
		return false;
	}
	
	UE_LOG(LogRKConditionTree, Log,
		TEXT("[RKConditionTree] Evaluated %d condition(s) with operator '%s' — result: %s"),
		Conditions.Num(),
		*UEnum::GetValueAsString(RootOperator),
		Result ? TEXT("true") : TEXT("false"));
	
	return Result;
}

bool URKConditionTree::EvaluateAND(const FRKEventPayload& Payload) const
{
	for (const TObjectPtr<URKConditionBase>& Condition : Conditions)
	{
		if (!Condition)
		{
			UE_LOG(LogRKConditionTree, Warning,
				TEXT("[RKConditionTree] Null condition found in array — skipping."));
			continue;
		}
		if (!Condition->Evaluate(Payload)) return false;
	}
	
	return true;
}

bool URKConditionTree::EvaluateOR(const FRKEventPayload& Payload) const
{
	for (const TObjectPtr<URKConditionBase>& Condition : Conditions)
	{
		if (!Condition)
		{
			UE_LOG(LogRKConditionTree, Warning,
				TEXT("[RKConditionTree] Null condition found in array — skipping."));
			continue;
		}
		if (Condition->Evaluate(Payload)) return true;
	}
	
	return false;
}

bool URKConditionTree::EvaluateNOT(const FRKEventPayload& Payload) const
{
	if (!Conditions[0])
	{
		UE_LOG(LogRKConditionTree, Warning,
			TEXT("[RKConditionTree] NOT operator — first condition is null. Returning false."));
		return false;
	}
	
	if (Conditions.Num() > 1)
	{
		UE_LOG(LogRKConditionTree, Warning,
			TEXT("[RKConditionTree] NOT operator — only the first condition is evaluated. %d condition(s) ignored."),
			Conditions.Num() - 1);
	}
	
	return !Conditions[0]->Evaluate(Payload);
}

URKConditionTree* URKConditionTree::CreateConditionTree(UObject* Outer)
{
	return NewObject<URKConditionTree>(Outer);
}
