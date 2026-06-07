#include "Actions/RKActionBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogRKAction, Log, All);

void URKActionBase::Execute_Implementation(const FRKEventPayload& Payload)
{
	// Base implementation does nothing.
	// Subclasses must override this to provide meaningful action logic.
	UE_LOG(LogRKAction, Warning,
		TEXT("[RKAction] Execute called on base URKActionBase — subclass should override this."));
}
