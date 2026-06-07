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

FDelegateHandle URKDispatcherSubsystem::RegisterListener(FName EventName, const FRKEventDelegate::FDelegate& Delegate)
{
	if (EventName.IsNone())
	{
		UE_LOG(LogRKDispatcher, Warning,
			TEXT("[RKDispatcher] RegisterListener called with empty EventName — ignored."));
		return FDelegateHandle();
	}
	
	// FindOrAdd creates the entry if it does not existe yet.
	FRKEventDelegate& MulticastDelegate = ListenerRegistry.FindOrAdd(EventName);
	FDelegateHandle Handle = MulticastDelegate.Add(Delegate);
	
	UE_LOG(LogRKDispatcher, Log,
		TEXT("[RKDispatcher] Listener registered — event: '%s'."),
		*EventName.ToString());

	return Handle;
}

void URKDispatcherSubsystem::UnregisterListener(FName EventName, FDelegateHandle Handle)
{
	if (EventName.IsNone())
	{
		UE_LOG(LogRKDispatcher, Warning,
			TEXT("[RKDispatcher] UnregisterListener called with empty EventName — ignored."));
		return;
	}
	
	if (!Handle.IsValid())
	{
		UE_LOG(LogRKDispatcher, Warning,
			TEXT("[RKDispatcher] UnregisterListener called with invalid handle — ignored."));
		return;
	}
	
	FRKEventDelegate* MulticastDelegate = ListenerRegistry.Find(EventName);
	if (!MulticastDelegate)
	{
		// Event name not in registry — no-op, no crash
		UE_LOG(LogRKDispatcher, Log,
			TEXT("[RKDispatcher] UnregisterListener — event '%s' not found in registry, nothing to remove."),
			*EventName.ToString());
		return;
	}
	
	MulticastDelegate->Remove(Handle);
	
	UE_LOG(LogRKDispatcher, Log,
		TEXT("[RKDispatcher] Listener unregistered — event: '%s'."),
		*EventName.ToString());
}

void URKDispatcherSubsystem::DispatchEvent(FName EventName, const FRKEventPayload& Payload)
{
	if (EventName.IsNone())
	{
		UE_LOG(LogRKDispatcher, Warning,
			TEXT("[RKDispatcher] DispatchEvent called with empty EventName — ignored."));
		return;
	}
	
	// Broadcast to dynamic multicast - Blueprint listeners
	OnEventDispatched.Broadcast(Payload);
	
	// Broadcast to C++ listeners registered by event name
	FRKEventDelegate* MulticastDelegate = ListenerRegistry.Find(EventName);
	if (!MulticastDelegate)
	{
		UE_LOG(LogRKDispatcher, Log,
			TEXT("[RKDispatcher] DispatchEvent — no C++ listeners for event '%s'."),
			*EventName.ToString());
		return;
	}
	
	if (!MulticastDelegate->IsBound())
	{
		UE_LOG(LogRKDispatcher, Log,
			TEXT("[RKDispatcher] DispatchEvent — delegate for event '%s' has no bound listeners."),
			*EventName.ToString());
		return;
	}
	
	MulticastDelegate->Broadcast(Payload);
	
	UE_LOG(LogRKDispatcher, Log,
		TEXT("[RKDispatcher] Event dispatched — name: '%s' — instigator: '%s'."),
		*EventName.ToString(),
		Payload.Instigator ? *Payload.Instigator->GetName() : TEXT("None"));
}
