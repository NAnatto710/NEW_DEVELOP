#include "LGTCollisionToolModule.h"

#include "Framework/Application/SlateApplication.h"
#include "Framework/Commands/UIAction.h"
#include "Framework/Docking/TabManager.h"
#include "LevelEditor.h"
#include "SLGTCollisionToolWindow.h"
#include "ToolMenus.h"
#include "Widgets/Docking/SDockTab.h"

#define LOCTEXT_NAMESPACE "FLGTCollisionToolModule"

const FName FLGTCollisionToolModule::ToolTabName(TEXT("LGTCollisionTool"));

void FLGTCollisionToolModule::StartupModule()
{
    FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
        ToolTabName,
        FOnSpawnTab::CreateRaw(this, &FLGTCollisionToolModule::SpawnToolTab))
        .SetDisplayName(LOCTEXT("ToolTabTitle", "LGT Collision Tool"))
        .SetTooltipText(LOCTEXT("ToolTabTooltip", "Landscape Grass Tool の配置結果から Collision 専用 HISM を生成します。"))
        .SetMenuType(ETabSpawnerMenuType::Hidden);

    UToolMenus::RegisterStartupCallback(
        FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FLGTCollisionToolModule::RegisterMenus));
}

void FLGTCollisionToolModule::ShutdownModule()
{
    UToolMenus::UnRegisterStartupCallback(this);
    UToolMenus::UnregisterOwner(this);

    if (FSlateApplication::IsInitialized())
    {
        FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(ToolTabName);
    }
}

void FLGTCollisionToolModule::RegisterMenus()
{
    FToolMenuOwnerScoped OwnerScoped(this);

    UToolMenu* ToolsMenu = UToolMenus::Get()->ExtendMenu(TEXT("LevelEditor.MainMenu.Tools"));
    FToolMenuSection& Section = ToolsMenu->FindOrAddSection(TEXT("LGTCollisionTool"));

    Section.AddMenuEntry(
        TEXT("OpenLGTCollisionTool"),
        LOCTEXT("OpenToolLabel", "LGT Collision Tool"),
        LOCTEXT("OpenToolTooltip", "LGT の木を Collision 専用 HISM へ焼き付けます。"),
        FSlateIcon(),
        FUIAction(FExecuteAction::CreateRaw(this, &FLGTCollisionToolModule::OpenToolWindow)));
}

void FLGTCollisionToolModule::OpenToolWindow()
{
    FGlobalTabmanager::Get()->TryInvokeTab(ToolTabName);
}

TSharedRef<SDockTab> FLGTCollisionToolModule::SpawnToolTab(const FSpawnTabArgs& SpawnTabArgs)
{
    return SNew(SDockTab)
        .TabRole(ETabRole::NomadTab)
        [
            SNew(SLGTCollisionToolWindow)
        ];
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FLGTCollisionToolModule, LGTCollisionTool)
