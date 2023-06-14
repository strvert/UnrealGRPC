#pragma once

class FGRPCGeneratorUtils
{
public:
	static void GenerateGRPCClientCode(const FString& PluginName,
	                                   const FString& PluginDirectory,
	                                   const TArray<FString>& ProtoFilePaths);

	static FString GetProtocPath();
	static FString GetGrpcCppPluginPath();

private:
	static FString GetThirdPartyBinariesPath();
	static TTuple<FString, FString> GetGeneratedCodeDirectories(const FString& PluginName, const FString& PluginDirectory);
};
