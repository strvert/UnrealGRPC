#include "UnrealGrpcEditor.h"

#include "SGRPCClientPluginGenerator.h"
#include "ToolMenus.h"
#include "Widgets/Docking/SDockTab.h"

#define LOCTEXT_NAMESPACE "FUnrealGrpcEditorModule"

static const FName UnrealGrpcPluginGeneratorTabName = FName(TEXT("UnrealGrpcPluginGenerator"));

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FUnrealGrpcEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TSharedRef<SDockTab> CreateUnrealGrpcPluginGeneratorTab(const FSpawnTabArgs& SpawnTabArgs);
	static void RegisterToolMenu();
	static void UnregisterToolMenu();

	TSharedPtr<SDockTab> GeneratorTab;
};

void FUnrealGrpcEditorModule::StartupModule()
{
	FCoreDelegates::OnPostEngineInit.AddStatic(&FUnrealGrpcEditorModule::RegisterToolMenu);

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		                        UnrealGrpcPluginGeneratorTabName,
		                        FOnSpawnTab::CreateRaw(
			                        this, &FUnrealGrpcEditorModule::CreateUnrealGrpcPluginGeneratorTab))
	                        .SetAutoGenerateMenuEntry(false);
}

void FUnrealGrpcEditorModule::ShutdownModule()
{
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(UnrealGrpcPluginGeneratorTabName);
}

void FUnrealGrpcEditorModule::RegisterToolMenu()
{
	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Tools");
	FToolMenuSection& Section
		= Menu->AddSection("UnrealGrpcSection", LOCTEXT("UnrealGrpcSection_Label", "Unreal gRPC"));
	Section.AddMenuEntry(
		"GRPCClientGenerator",
		LOCTEXT("GRPCClientGenerator", "gRPC Client Generator"),
		LOCTEXT("GRPCClientGeneratorTooltip", "Generate gRPC client code from proto files"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateLambda([]()
		{
			FGlobalTabmanager::Get()->TryInvokeTab(UnrealGrpcPluginGeneratorTabName);
		})),
		EUserInterfaceActionType::Button
	);
}

void FUnrealGrpcEditorModule::UnregisterToolMenu()
{
	UToolMenus::Get()->RemoveSection("LevelEditor.MainMenu.Tools", "UnrealGrpcSection");
}

TSharedRef<SDockTab> FUnrealGrpcEditorModule::CreateUnrealGrpcPluginGeneratorTab(
	const FSpawnTabArgs& SpawnTabArgs)
{
	SAssignNew(GeneratorTab, SDockTab).TabRole(ETabRole::NomadTab);
	GeneratorTab->SetContent(SNew(SGRPCClientPluginGenerator, GeneratorTab));
	return GeneratorTab.ToSharedRef();
}

DEFINE_LOG_CATEGORY(LogUnrealGrpcEditor);

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUnrealGrpcEditorModule, UnrealGrpcEditor)
