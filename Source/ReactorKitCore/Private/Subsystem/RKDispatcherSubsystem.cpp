#include "Subsystem/RKDispatcherSubsystem.h"

DEFINE_LOG_CATEGORY_STATIC(LogRKDispatcher, Log, All);

void URKDispatcherSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	UE_LOG(LogRKDispatcher, Log,
		TEXT("[RKDispatcher] Subsystem initialized."));
}

void URKDispatcherSubsystem::Deinitialize()
{
	ListenerRegistry.Empty();
	
	UE_LOG(LogRKDispatcher, Log,
		TEXT("[RKDispatcher] Subsystem deinitialized — registry cleared."));
	
	Super::Deinitialize();
}

void URKDispatcherSubsystem::RegisterListener(FName EventName, const FRKEventDelegate::FDelegate& Delegate)
{
	// STUB — full implementation in Issue #10
	UE_LOG(LogRKDispatcher, Log,
		TEXT("[RKDispatcher] RegisterListener called — event: '%s' (stub)."),
		*EventName.ToString());
}

void URKDispatcherSubsystem::UnregisterListener(FName EventName, FDelegateHandle Handle)
{
	// STUB — full implementation in Issue #10
	UE_LOG(LogRKDispatcher, Log,
		TEXT("[RKDispatcher] UnregisterListener called — event: '%s' (stub)."),
		*EventName.ToString());
}

void URKDispatcherSubsystem::DispatchEvent(FName EventName, const FRKEventPayload& Payload)
{
	// STUB — full implementation in Issue #10
	UE_LOG(LogRKDispatcher, Log,
		TEXT("[RKDispatcher] DispatchEvent called — event: '%s' — instigator: '%s' (stub)."),
		*EventName.ToString(),
		Payload.Instigator ? *Payload.Instigator->GetName() : TEXT("None"));
}
