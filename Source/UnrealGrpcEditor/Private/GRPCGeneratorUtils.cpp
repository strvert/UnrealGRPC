#include "GRPCGeneratorUtils.h"

#include "Interfaces/IPluginManager.h"
#include "UnrealGrpcEditor.h"

void FGRPCGeneratorUtils::GenerateGRPCClientCode(
	const FString& PluginName, const FString& PluginDirectory, const TArray<FString>& ProtoFilePaths)
{
	// Get paths
	const auto& [PublicCodeDirectory, PrivateCodeDirectory]
		= GetGeneratedCodeDirectories(PluginName, PluginDirectory);

	// Create directories
	IFileManager::Get().MakeDirectory(*PublicCodeDirectory, true);
	IFileManager::Get().MakeDirectory(*PrivateCodeDirectory, true);

	// Generate code
	const FString& ProtocPath = GetProtocPath();
	const FString& GrpcCppPluginPath = GetGrpcCppPluginPath();

	const FString& Command = FString::Printf(
		TEXT("--proto_path=\"%s\" --cpp_out=\"%s\" --grpc_out=\"%s\" --plugin=protoc-gen-grpc=\"%s\" %s"),
		*FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / TEXT("Resources") / TEXT("Proto")),
		*PrivateCodeDirectory,
		*PrivateCodeDirectory,
		*GrpcCppPluginPath,
		*FString::Join(ProtoFilePaths, TEXT(" ")));

	UE_LOG(LogUnrealGrpcEditor, Log, TEXT("Running command: %s %s"), *ProtocPath, *Command);

	int32 ReturnCode;
	FString StdOut;
	FString StdErr;
	FPlatformProcess::ExecProcess(*ProtocPath, *Command, &ReturnCode, &StdOut, &StdErr);

	if (!StdOut.IsEmpty())
	{
		UE_LOG(LogUnrealGrpcEditor, Log, TEXT("%s"), *StdOut);
	}
	if (!StdErr.IsEmpty())
	{
		UE_LOG(LogUnrealGrpcEditor, Error, TEXT("%s"), *StdErr);
	}

	if (ReturnCode != 0)
	{
		UE_LOG(LogUnrealGrpcEditor, Error, TEXT("Failed to generate code for plugin %s"), *PluginName);
		IFileManager::Get().DeleteDirectory(*PublicCodeDirectory, false, true);
		IFileManager::Get().DeleteDirectory(*PrivateCodeDirectory, false, true);
	}
	else
	{
		// copy .h files from private to public
		TArray<FString> Files;
		IFileManager::Get().FindFilesRecursive(Files, *PrivateCodeDirectory, TEXT("*.h"), true, false);
		for (const FString& File : Files)
		{
			const FString& PublicFile = PublicCodeDirectory / FPaths::GetCleanFilename(File);
			IFileManager::Get().Copy(*PublicFile, *File);
		}
	}
}

FString FGRPCGeneratorUtils::GetProtocPath()
{
#if PLATFORM_WINDOWS
	return GetThirdPartyBinariesPath() / TEXT("protoc.exe");
#else
	// Not implemented
	return TEXT("");
#endif
}

FString FGRPCGeneratorUtils::GetGrpcCppPluginPath()
{
#if PLATFORM_WINDOWS
	return GetThirdPartyBinariesPath() / TEXT("grpc_cpp_plugin.exe");
#else
	return TEXT("");
#endif
}

FString FGRPCGeneratorUtils::GetThirdPartyBinariesPath()
{
	const FString& SelfPluginPath = FPaths::ConvertRelativePathToFull(
		IPluginManager::Get().FindPlugin(TEXT("UnrealGrpc"))->GetBaseDir());
	const FString& ThirdPartyPath = SelfPluginPath / TEXT("Source") / TEXT("ThirdParty");
	return ThirdPartyPath / TEXT("grpc") / TEXT("bin") / FPlatformProcess::GetBinariesSubdirectory();
}

TTuple<FString, FString> FGRPCGeneratorUtils::GetGeneratedCodeDirectories(
	const FString& PluginName, const FString& PluginDirectory)
{
	const FString& PluginSourceDirectory = PluginDirectory / PluginName / TEXT("Source") / PluginName;

	const FString& PluginPublicDirectory = FPaths::ConvertRelativePathToFull(PluginSourceDirectory / TEXT("Public"));
	const FString& PluginPrivateDirectory = FPaths::ConvertRelativePathToFull(PluginSourceDirectory / TEXT("Private"));

	return {(PluginPublicDirectory / TEXT("Proto")), (PluginPrivateDirectory / TEXT("Proto"))};
}
