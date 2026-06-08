#include "Conditions/RKCustomCondition.h"

DEFINE_LOG_CATEGORY_STATIC(LogRKCustomCondition, Log, All);

bool URKCustomCondition::Evaluate_Implementation(const FRKEventPayload& Payload) const
{
	// URKCustomCondition has no default evaluation logic.
	// This class must be subclassed in Blueprint or C++ before use.
	UE_LOG(LogRKCustomCondition, Error,
		TEXT("[RKCustomCondition] Evaluate called directly on URKCustomCondition — ")
		TEXT("this class must be subclassed. Override Evaluate in a Blueprint or C++ child. Returning false."));

	return false;
}
