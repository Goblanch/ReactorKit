#pragma once

#include "Modules/ModuleManager.h"

class FReactorKitCoreModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};