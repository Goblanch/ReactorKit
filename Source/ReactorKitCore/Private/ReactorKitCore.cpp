#include "ReactorKitCore.h"

#define LOCTEXT_NAMESPACE "FReactorKitCoreModule"

void FReactorKitCoreModule::StartupModule()
{
	UE_LOG(LogTemp, Log, TEXT("[ReactorKitCore] Module started."));
}

void FReactorKitCoreModule::ShutdownModule()
{
	UE_LOG(LogTemp, Log, TEXT("[ReactorKitCore] Module shut down."));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FReactorKitCoreModule, ReactorKitCore)