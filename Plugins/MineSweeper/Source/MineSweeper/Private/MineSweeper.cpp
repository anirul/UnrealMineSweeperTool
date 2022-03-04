// Copyright Epic Games, Inc. All Rights Reserved.

#include "MineSweeper.h"
#include "MineSweeperStyle.h"
#include "MineSweeperCommands.h"
#include "MineSweeperWidget.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "MainFrame/Public/Interfaces/IMainFrameModule.h"

static const FName MineSweeperTabName("MineSweeper");

#define LOCTEXT_NAMESPACE "FMineSweeperModule"

void FMineSweeperModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FMineSweeperStyle::Initialize();
	FMineSweeperStyle::ReloadTextures();

	FMineSweeperCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FMineSweeperCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FMineSweeperModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMineSweeperModule::RegisterMenus));
}

void FMineSweeperModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FMineSweeperStyle::Shutdown();

	FMineSweeperCommands::Unregister();
}

void FMineSweeperModule::PluginButtonClicked()
{
    TSharedRef<SWindow> MineSweeperWindow = SNew(SWindow)
        .Title(FText::FromString(TEXT("Mine Sweeper Window")))
        .ClientSize(FVector2D(800, 400))
        .SupportsMaximize(false)
        .SupportsMinimize(false)
        [
            SNew(SMineSweeperWidget)
        ];
	IMainFrameModule& MainFrameModule =
		FModuleManager::LoadModuleChecked<IMainFrameModule>(TEXT("MainFrame"));
	if (MainFrameModule.GetParentWindow().IsValid())
	{
		FSlateApplication::Get().AddWindowAsNativeChild(
			MineSweeperWindow,
			MainFrameModule.GetParentWindow().ToSharedRef());
	}
	else
	{
		FSlateApplication::Get().AddWindow(MineSweeperWindow);
	}
}

void FMineSweeperModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FMineSweeperCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FMineSweeperCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMineSweeperModule, MineSweeper)