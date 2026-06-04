#pragma once

#include "Modules/ModuleManager.h"

class FReactorKitEditorModule : public IModuleInterface
{
public:
    virtual void StartUpModule() override;
    virtual void ShutDownModule() override;
};