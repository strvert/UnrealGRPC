#include "SGRPCClientPluginGenerator.h"

#include "GRPCGeneratorUtils.h"
#include "NewGRPCClientPluginDescriptorData.h"
#include "PluginUtils.h"
#include "Widgets/Docking/SDockTab.h"
#include "PropertyEditorModule.h"
#include "SPrimaryButton.h"

#define LOCTEXT_NAMESPACE "SGRPCClientPluginGenerator"

static const FString GRPCClientPluginDir
	= FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("GeneratedGRPCClients"));

void SGRPCClientPluginGenerator::Construct(const FArguments& InArgs, const TSharedPtr<SDockTab> InOwnerTab)
{
	OwnerTab = InOwnerTab;

	FPropertyEditorModule& PropertyEditorModule
		= FModuleManager::Get().GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FDetailsViewArgs DetailsViewArgs;
	{
		DetailsViewArgs.bAllowSearch = false;
		DetailsViewArgs.bShowOptions = false;
		DetailsViewArgs.bAllowMultipleTopLevelObjects = false;
		DetailsViewArgs.bAllowFavoriteSystem = false;
		DetailsViewArgs.bShowObjectLabel = false;
		DetailsViewArgs.bHideSelectionTip = true;
	}

	const TSharedPtr<IDetailsView> GeneratorPropertyView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);

	DescriptorData = NewObject<UNewGRPCClientPluginDescriptorData>();

	const TSharedRef<SVerticalBox> MainContent = SNew(SVerticalBox);

	if (DescriptorData.IsValid())
	{
		GeneratorPropertyView->SetObject(DescriptorData.Get());
		MainContent->AddSlot()
		[
			GeneratorPropertyView.ToSharedRef()
		];
	}

	MainContent->AddSlot()
	           .AutoHeight()
	           .Padding(10)
	           .HAlign(HAlign_Right)
	[
		SNew(SPrimaryButton)
		.Text(LOCTEXT("GenerateButtonLabel", "Generate"))
		.OnClicked(this, &SGRPCClientPluginGenerator::OnGenerateButtonClicked)
		.IsEnabled_Raw(this, &SGRPCClientPluginGenerator::IsGenerateButtonEnabled)
	];

	ChildSlot[
		MainContent
	];
}

FReply SGRPCClientPluginGenerator::OnGenerateButtonClicked() const
{
	if (!DescriptorData.IsValid())
	{
		return FReply::Handled();
	}
	const UNewGRPCClientPluginDescriptorData& Data = *DescriptorData.Get();
	const FString PluginDir = GRPCClientPluginDir;
	const FPluginUtils::FNewPluginParamsWithDescriptor& NewPluginParams = Data.ToNewPluginParams();

	FPluginUtils::FLoadPluginParams LoadPluginParams;
	{
		LoadPluginParams.bSynchronousAssetsScan = false;
		LoadPluginParams.bEnablePluginInProject = true;
		LoadPluginParams.bSelectInContentBrowser = false;
		LoadPluginParams.bUpdateProjectPluginSearchPath = false;
	}

	TArray<FString> ProtoFilePathStrings;
	Algo::Transform(Data.ProtoFiles, ProtoFilePathStrings, [](const FFilePath& ProtoFilePath)
	{
		return FPaths::GetCleanFilename(ProtoFilePath.FilePath);
	});
	FGRPCGeneratorUtils::GenerateGRPCClientCode(Data.Name, PluginDir, ProtoFilePathStrings);

	const TSharedPtr<IPlugin> NewPlugin
		= FPluginUtils::CreateAndLoadNewPlugin(Data.Name, PluginDir, NewPluginParams, LoadPluginParams);

	if (NewPlugin.IsValid() && OwnerTab.IsValid())
	{
		const TSharedPtr<SDockTab> OwnerTabPtr = OwnerTab.Pin();
		OwnerTabPtr->RequestCloseTab();
	}
	
	return FReply::Handled();
}

bool SGRPCClientPluginGenerator::IsGenerateButtonEnabled() const
{
	if (DescriptorData.IsValid())
	{
		const auto& Data = *DescriptorData.Get();
		return !Data.Name.IsEmpty() && Data.ValidateProtoFilePaths();
	}
	return false;
}

#undef LOCTEXT_NAMESPACE
