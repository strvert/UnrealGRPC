// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PluginUtils.h"
#include "UObject/Object.h"
#include "NewGRPCClientPluginDescriptorData.generated.h"

UCLASS()
class UNREALGRPCEDITOR_API UNewGRPCClientPluginDescriptorData : public UObject
{
	GENERATED_BODY()

public:
	UNewGRPCClientPluginDescriptorData(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, Category = "Descriptor Data", meta = (DisplayName = "Plugin Name"))
	FString Name;

	UPROPERTY(EditAnywhere, Category = "Descriptor Data", meta = (DisplayName = "gRPC Proto Files", FilePathFilter = "proto", RelativeToGameDir))
	TArray<FFilePath> ProtoFiles;

	UPROPERTY(EditAnywhere, Category = "Descriptor Data", meta = (DisplayName = "Author"))
	FString CreatedBy;

	UPROPERTY(EditAnywhere, Category = "Descriptor Data", AdvancedDisplay, meta = (DisplayName = "Author URL"))
	FString CreatedByURL;

	UPROPERTY(EditAnywhere, Category="Descriptor Data")
	FString Description;
	
	UPROPERTY(EditAnywhere, Category="Descriptor Data", AdvancedDisplay)
	bool bIsBetaVersion = false;

	bool ValidateProtoFilePaths() const;

	FPluginUtils::FNewPluginParamsWithDescriptor ToNewPluginParams() const;
};
