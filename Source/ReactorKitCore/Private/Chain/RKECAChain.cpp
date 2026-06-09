#include "Chain/RKECAChain.h"

DEFINE_LOG_CATEGORY_STATIC(LogRKECAChain, Log, All);

void URKECAChain::Execute(const FRKEventPayload& Payload)
{
	if (bIsExecuting)
	{
		UE_LOG(LogRKECAChain, Warning,
			TEXT("[RKECAChain] Execute called while already executing — call Interrupt() first. Ignoring."));
		return;
	}
	
	if (Actions.IsEmpty())
	{
		UE_LOG(LogRKECAChain, Log,
			TEXT("[RKECAChain] No actions configured — nothing to execute."));
		return;
	}
	
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogRKECAChain, Warning,
			TEXT("[RKECAChain] No valid UWorld found — cannot manage timers. Aborting."));
		return;
	}
	
	bIsExecuting = true;
	ActivePayload = Payload;
	PendingTimers.Reset();
	
	UE_LOG(LogRKECAChain, Log,
		TEXT("[RKECAChain] Starting execution — %d action(s) in chain."),
		Actions.Num());
	
	// Schedule each action - zero delay actions execute via timer on next frame
	// to keep execution order consistent regardless of delay value
	float AccumulatedDelay = 0.0f;
	
	for (int32 i = 0; i < Actions.Num(); i++)
	{
		if (!Actions[i])
		{
			UE_LOG(LogRKECAChain, Warning,
				TEXT("[RKECAChain] Null action at index %d — skipping."), i);
			continue;
		}
		
		AccumulatedDelay += Actions[i]->DelayBeforeExecution;
		
		FTimerHandle Handle;
		const int32 ActionIndex = i;
		
		if (AccumulatedDelay <= 0.0f)
		{
			// Execute immediately on next frame via a minimal timer
			World->GetTimerManager().SetTimer(
				Handle,
				[this, ActionIndex]() { ExecuteActionAtIndex(ActionIndex); },
				SMALL_NUMBER,
				false);
		}else
		{
			World->GetTimerManager().SetTimer(
				Handle,
				[this, ActionIndex]() { ExecuteActionAtIndex(ActionIndex); },
				AccumulatedDelay,
				false);
		}
		
		PendingTimers.Add(Handle);
	}
}

void URKECAChain::Interrupt()
{
	if (!bIsExecuting) return;
	
	ClearAllTimers();
	bIsExecuting = false;
	
	UE_LOG(LogRKECAChain, Log,
		TEXT("[RKECAChain] Chain interrupted — all pending timers cleared."));
}

URKECAChain* URKECAChain::CreateECAChain(UObject* Outer)
{
	return NewObject<URKECAChain>(Outer);
}

void URKECAChain::ExecuteActionAtIndex(int32 ActionIndex)
{
	if (!bIsExecuting) return;
	
	if (!Actions.IsValidIndex(ActionIndex) || !Actions[ActionIndex])
	{
		UE_LOG(LogRKECAChain, Warning,
			TEXT("[RKECAChain] Action at index %d is invalid or null — skipping."),
			ActionIndex);
		return;
	}
	
	UE_LOG(LogRKECAChain, Log,
		TEXT("[RKECAChain] Executing action %d / %d — class: '%s'."),
		ActionIndex + 1,
		Actions.Num(),
		*Actions[ActionIndex]->GetClass()->GetName());
	
	Actions[ActionIndex]->Execute(ActivePayload);
	
	const bool bIsLastAction = (ActionIndex == Actions.Num() - 1);
	
	// Check if next actions are null
	bool bAllSubsequentNull = true;
	for (int32 i = ActionIndex + 1; i < Actions.Num(); i++)
	{
		if (Actions[i])
		{
			bAllSubsequentNull = false;
			break;
		}
	}
	
	if (bIsLastAction || bAllSubsequentNull)
	{
		bIsExecuting = false;
		PendingTimers.Reset();
		
		UE_LOG(LogRKECAChain, Log,
			TEXT("[RKECAChain] Chain execution complete."));
	}
}

void URKECAChain::ClearAllTimers()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		PendingTimers.Reset();
		return;
	}
	
	for (FTimerHandle& Handle : PendingTimers)
	{
		if (Handle.IsValid())
		{
			World->GetTimerManager().ClearTimer(Handle);
		}
	}
	
	PendingTimers.Reset();
}

UWorld* URKECAChain::GetWorld() const
{
	if (const UObject* Outer = GetOuter())
	{
		return Outer->GetWorld();
	}
	return nullptr;
}
