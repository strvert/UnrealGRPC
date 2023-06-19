// Copyright 2022 WandererDev. All Rights Reserved.

using System;
using System.IO;
using UnrealBuildTool;
using EpicGames.Core;


public class Grpc : ModuleRules
{
	public Grpc(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		Log.TraceInformation("Platform={0}, Architecture={1}", Target.Platform.ToString(), Target.Architecture);

		string GrpcLibDir = Path.Combine(ModuleBuildPath, "lib", Target.Platform.ToString());
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			AddEngineThirdPartyPrivateStaticDependencies(Target, "zlib");
			AddPublicLibrary(Target, Path.Combine(GrpcLibDir, "Release"));
			PublicDefinitions.Add("__SIZEOF_INT128__=0");
			PublicDefinitions.Add("PROTOBUF_BUILTIN_ATOMIC=0");
		}
		else if (Target.Platform == UnrealTargetPlatform.Linux)
		{
			AddEngineThirdPartyPrivateStaticDependencies(Target, "zlib");
			AddPublicLibrary(Target, Path.Combine(GrpcLibDir, "Release"));
		}
		else
		{
			Log.TraceError("Grpc: No support library: libPath={0}", GrpcLibDir);
			return;
		}

		// gRPC include
		PublicIncludePaths.Add(Path.Combine(ModuleBuildPath, "include"));
		PublicIncludePaths.Add(Path.Combine(ModuleBuildPath, "protobuf", "src"));
		PublicIncludePaths.Add(Path.Combine(ModuleBuildPath, "abseil-cpp"));

		// gRPC define
		PublicDefinitions.Add("GOOGLE_PROTOBUF_INTERNAL_DONATE_STEAL_INLINE=0");
		PublicDefinitions.Add("GPR_FORBID_UNREACHABLE_CODE=0");
		PublicDefinitions.Add("PROTOBUF_ENABLE_DEBUG_LOGGING_MAY_LEAK_PII=0");
	}

	protected virtual bool AddPublicLibrary(ReadOnlyTargetRules Target, string libraryDir)
	{
		string[] libraryPaths;

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			libraryPaths = Directory.GetFiles(libraryDir, "*.lib");
		}
		else
		{
			libraryPaths = Directory.GetFiles(libraryDir, "*.a");
		}


		if (libraryPaths.Length == 0)
		{
			return false;
		}
		else
		{
			PublicAdditionalLibraries.AddRange(libraryPaths);
			return true;
		}
	}

	protected virtual bool AddPublicIncludePath(string includeDir)
	{
		return true;
	}

	protected virtual string ModuleBuildPath
	{
		get { return Path.Combine(ModuleDirectory, "grpc"); }
	}

	protected virtual string EngineThirdPartyPath
	{
		get { return Path.Combine(EngineDirectory, "Source", "ThirdParty"); }
	}
}