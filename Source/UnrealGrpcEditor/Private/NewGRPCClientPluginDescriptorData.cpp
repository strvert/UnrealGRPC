// Fill out your copyright notice in the Description page of Project Settings.


#include "NewGRPCClientPluginDescriptorData.h"

#include "Interfaces/IPluginManager.h"

UNewGRPCClientPluginDescriptorData::UNewGRPCClientPluginDescriptorData(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool UNewGRPCClientPluginDescriptorData::ValidateProtoFilePaths() const
{
	if (ProtoFiles.Num() == 0)
	{
		return false;
	}
	for (const FFilePath& ProtoFilePath : ProtoFiles)
	{
		if (ProtoFilePath.FilePath.IsEmpty())
		{
			return false;
		}
	}
	return true;
}

FPluginUtils::FNewPluginParamsWithDescriptor UNewGRPCClientPluginDescriptorData::ToNewPluginParams() const
{
	const TSharedPtr<IPlugin> SelfPlugin = IPluginManager::Get().FindPlugin(TEXT("UnrealGrpc"));

	FPluginUtils::FNewPluginParamsWithDescriptor Params;
	{
		Params.Descriptor.CreatedBy = CreatedBy;
		Params.Descriptor.CreatedByURL = CreatedByURL;
		Params.Descriptor.Description = Description;
		Params.Descriptor.bIsBetaVersion = bIsBetaVersion;
		Params.Descriptor.FriendlyName = FString::Printf(TEXT("%s (Generated gRPC client plugin)"), *Name);
		Params.TemplateFolders.Emplace(FPaths::Combine(SelfPlugin->GetBaseDir(), TEXT("Templates"), TEXT("GRPCClient")));
		Params.Descriptor.Version = 1;
		Params.Descriptor.VersionName = TEXT("1.0");
		Params.Descriptor.Category = TEXT("Networking");
		Params.Descriptor.Plugins.Emplace(FPluginReferenceDescriptor(*SelfPlugin->GetName(), true));

		Params.Descriptor.Modules.Emplace(FModuleDescriptor(*Name, EHostType::Runtime, ELoadingPhase::Default));
	}
	return Params;
}
