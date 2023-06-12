#include "UnrealGrpcEditor.h"

#include "ToolMenus.h"

#define LOCTEXT_NAMESPACE "FUnrealGrpcEditorModule"

void FUnrealGrpcEditorModule::StartupModule()
{
	// FCoreDelegates::OnPostEngineInit.AddRaw(this, &FUnrealGrpcEditorModule::RegisterToolMenu);
	RegisterToolMenu();
}

void FUnrealGrpcEditorModule::ShutdownModule()
{
    
}

void FUnrealGrpcEditorModule::RegisterToolMenu()
{
	if (UToolMenus* ToolMenus = UToolMenus::Get())
	{
		UToolMenu* Menu = ToolMenus->ExtendMenu("LevelEditor.MainMenu.Tools");
		FToolMenuSection Section = Menu->AddSection("UnrealGrpc", LOCTEXT("UnrealGrpc", "UnrealGrpc"));

		Section.AddMenuEntry(
			"GRPCClientGenerator",
			LOCTEXT("GRPCClientGenerator", "gRPC Client Generator"),
			LOCTEXT("GRPCClientGeneratorTooltip", "Generate gRPC client code from proto files"),
			FSlateIcon(),
			FUIAction(FExecuteAction::CreateLambda([]() {
				UE_LOG(LogTemp, Warning, TEXT("UnrealGrpc"));
			}))
		);
	}
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FUnrealGrpcEditorModule, UnrealGrpcEditor)