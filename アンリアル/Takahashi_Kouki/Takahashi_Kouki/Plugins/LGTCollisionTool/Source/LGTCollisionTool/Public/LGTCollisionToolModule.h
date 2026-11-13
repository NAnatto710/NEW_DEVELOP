#pragma once

#include "Modules/ModuleManager.h"

class SDockTab;
class FSpawnTabArgs;

class FLGTCollisionToolModule final : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:
    void RegisterMenus();
    void OpenToolWindow();
    TSharedRef<SDockTab> SpawnToolTab(const FSpawnTabArgs& SpawnTabArgs);

    static const FName ToolTabName;
};
