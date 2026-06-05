#include "Conditions/RKConditionBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogRKCondition, Log, All);

bool URKConditionBase::Evaluate_Implementation(const FRKEventPayload& Payload) const
{
	UE_LOG(LogRKCondition, Warning,
		TEXT("[RKCondition] Evaluate called on base URKConditionBase — subclass should override this. Returning true."));
	
	return true;
}
