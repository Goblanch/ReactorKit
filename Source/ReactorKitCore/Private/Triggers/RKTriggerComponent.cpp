#include "Triggers/RKTriggerComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogRKTrigger, Log, All);

URKTriggerComponent::URKTriggerComponent()
{
	// Trigger component do not need to tick by default.
	// Subclasses that require per-frame logic should enable this explicity.
	PrimaryComponentTick.bCanEverTick = false;
}

void URKTriggerComponent::FireTrigger_Implementation(const FRKEventPayload& Payload)
{
	if (!CanActivate()) return;
	
	if (const UWorld* World = GetWorld())
	{
		LastActivationTime = World->GetTimeSeconds();
	}
	ActivationCount++;
	
	UE_LOG(LogRKTrigger, Log, 
		TEXT("[RKTrigger] '%s' activated - count: %d / max: %d - event: '%s'"),
		*GetOwner()->GetName(),
		ActivationCount,
		MaxActivations,
		*Payload.EventName.ToString());
}

void URKTriggerComponent::EnableTrigger()
{
	bEnabled = true;
	
	UE_LOG(LogRKTrigger, Log,
		TEXT("[RKTrigger] '%s' enabled."),
		*GetOwner()->GetName());
}

void URKTriggerComponent::DisableTrigger()
{
	bEnabled = false;
	
	UE_LOG(LogRKTrigger, Log,
		TEXT("[RKTrigger] '%s' disabled."),
		*GetOwner()->GetName());
}

void URKTriggerComponent::ResetActivationState()
{
	ActivationCount = 0;
	LastActivationTime = -1.0f;
	
	UE_LOG(LogRKTrigger, Log,
		TEXT("[RKTrigger] '%s' activation state reset."),
		*GetOwner()->GetName());
}

bool URKTriggerComponent::CanActivate() const
{
	if (!bEnabled) return false;
	if (MaxActivations > 0 && ActivationCount >= MaxActivations)
	{
		UE_LOG(LogRKTrigger, Log,
			TEXT("[RKTrigger] '%s' blocked — max activations reached (%d)."),
			*GetOwner()->GetName(),
			MaxActivations);
		
		return false;
	}
	if (Cooldown > 0.0f && LastActivationTime >= 0.0f)
	{
		const UWorld* World = GetWorld();
		if (World)
		{
			const float TimeSinceLast = World->GetTimeSeconds() - LastActivationTime;
			if (TimeSinceLast < Cooldown) {
				UE_LOG(LogRKTrigger, Log,
									TEXT("[RKTrigger] '%s' blocked — cooldown active (%.2fs remaining)."),
									*GetOwner()->GetName(),
									Cooldown - TimeSinceLast);
				
				return false;
			}
		}
	}
	
	return true;
}
