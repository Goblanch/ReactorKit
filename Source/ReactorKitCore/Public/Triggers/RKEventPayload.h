#pragma once

#include "CoreMinimal.h"
#include "RKEventPayload.generated.h"

/**
 * Core data struct that travels through the entire ReactorKit ECA system.
 * Created by a Trigger on activation and passed through the Dispatcher,
 * ConditionTree, and ECAChain without modification.
 * 
 * CustomData allows passing arbitrary key-value string pairs to extend
 * the payload for specific use cases without breaking the API contract.
 */
USTRUCT(BlueprintType)
struct REACTORKITCORE_API FRKEventPayload
{
	GENERATED_BODY()
	
	/**
	 * Name that identifies which event was fired.
	 * Used by the DispatcherSubsystem to route the payload
	 * to correct registered listeners.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReactorKit|Event")
	FName EventName;
	
	/**
	 * The actor that originated or caused this event.
	 * For an overlap trigger, this is the actor that entered the volume.
	 * May be null if the event has no clear instigator.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReactorKit|Event")
	TObjectPtr<AActor> Instigator = nullptr;
	
	/**
	 * The actor that is the subject or target of this event.
	 * For example, the actor that owns the trigger that fired.
	 * May be null if the event has no specific targets.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReactorKit|Event")
	TObjectPtr<AActor> Target = nullptr;
	
	/**
	 * World position associated with this event.
	 * For an overlap trigger, this is the location of the overlap.
	 * Defaults to zero vector if location is not relevant.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReactorKit|Event")
	FVector Location = FVector::ZeroVector;
	
	/**
	 * Arbitrary key-value attached to this event.
	 * Allows passing custom data through the ECA pipeline without
	 * modifying the payload struct or breaking the API contract.
	 * Keys and values are both FName and FString respectively.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReactorKit|Event")
	TMap<FName, FString> CustomData;
	
};