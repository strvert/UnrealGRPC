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

        // Use UE third-party library
        PublicDependencyModuleNames.AddRange(new string[]
        {
            "OpenSSL",
            "zlib"
        });

        Log.TraceInformation("Platform={0}, Architecture={1}", Target.Platform.ToString(), Target.Architecture);

        string grpcLibDir = Path.Combine(ModuleBuildPath, "lib", Target.Platform.ToString().ToLower());
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            AddPublicLibrary(Target, Path.Combine(OpenSSLLib, "Win64", "VS2015", "Release"));
            AddPublicIncludePath(Path.Combine(OpenSSLInclude, "Win64", "VS2015"));

            AddPublicLibrary(Target, Path.Combine(grpcLibDir, "Release"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Android)
        {
            AddPublicLibrary(Target, Path.Combine(OpenSSLLib, "Android", "ARM64"));
            AddPublicIncludePath(Path.Combine(OpenSSLInclude, "Android"));


            AddPublicLibrary(Target, Path.Combine(grpcLibDir, "arm64-v8a"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            AddPublicLibrary(Target, Path.Combine(OpenSSLLib, "Mac"));
            AddPublicIncludePath(Path.Combine(OpenSSLInclude, "Mac"));

            AddPublicLibrary(Target, grpcLibDir);

            PublicSystemLibraries.Add("resolv");
        }
        else if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            AddPublicLibrary(Target, Path.Combine(OpenSSLLib, "IOS"));
            AddPublicIncludePath(Path.Combine(OpenSSLInclude, "IOS"));

            AddPublicLibrary(Target, grpcLibDir);

            PublicSystemLibraries.Add("resolv");
        }
        else if (Target.IsInPlatformGroup(UnrealPlatformGroup.Unix))
        {
            PCHUsage = ModuleRules.PCHUsageMode.Default;

            // Target.Architecture="x86_64-unknown-linux-gnu" or "aarch64-unknown-linux-gnueabi"
            // AddPublicLibrary(Target, Path.Combine(OpenSSLLib, "Unix", Target.Architecture));
            // AddPublicIncludePath(Path.Combine(OpenSSLInclude, "Unix", Target.Architecture));

            AddPublicLibrary(Target, grpcLibDir);

            AddPublicLibrary(Target, Path.Combine(UnixPath, "LibCxx", "lib", "Unix", "x86_64-unknown-linux-gnu"));
            AddPublicIncludePath(Path.Combine(UnixPath, "LibCxx", "include", "c++", "v1"));
        }
        else
        {
            Log.TraceError("Grpc: No support library: libPath={0}", grpcLibDir);
            return;
        }

        // gRPC include
        AddPublicIncludePath(Path.Combine(ModuleBuildPath, "include"));
        AddPublicIncludePath(Path.Combine(ModuleBuildPath, "protobuf", "src"));
        AddPublicIncludePath(Path.Combine(ModuleBuildPath, "abseil-cpp"));
        //AddPublicIncludePath(Path.Combine(ModuleProjectPath, "include"));
        //AddPublicIncludePath(Path.Combine(ModuleProjectPath, "third_party", "protobuf", "src"));
        //AddPublicIncludePath(Path.Combine(ModuleProjectPath, "third_party", "abseil-cpp"));

        // gRPC define
        PublicDefinitions.Add("GOOGLE_PROTOBUF_INTERNAL_DONATE_STEAL_INLINE=0");
        PublicDefinitions.Add("GPR_FORBID_UNREACHABLE_CODE=0");

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
            Log.TraceError("AddPublicLibrary: Can't find library in libraryDir={0}", libraryDir);
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
        PublicIncludePaths.Add(includeDir);
        Log.TraceInformation("AddPublicIncludePath: {0}", includeDir);

        return true;
    }


    protected virtual string ModuleProjectPath
    {
        get
        {
            return Path.Combine(ModuleDirectory, "..", "..", "ThirdParty", "grpc-1.51.1");
        }
    }

    protected virtual string ModuleBuildPath
    {
        get
        {
            return Path.Combine(ModuleDirectory, "grpc");
        }
    }

    protected virtual string EngineThirdPartyPath
    {
        get
        {
            return Path.Combine(EngineDirectory, "Source", "ThirdParty");
        }
    }

    protected virtual string OpenSSLPath
    {
        get
        {
            return Path.Combine(EngineThirdPartyPath, "OpenSSL");
        }
    }

    protected virtual string OpenSSLLib
    {
        get
        {
            return Path.Combine(EngineThirdPartyPath, "OpenSSL", "1.1.1n", "lib");
        }
    }

    protected virtual string OpenSSLInclude
    {
        get
        {
            return Path.Combine(EngineThirdPartyPath, "OpenSSL", "1.1.1n", "include");
        }
    }

    protected virtual string UnixPath
    {
        get
        {
            return Path.Combine(EngineThirdPartyPath, "Unix");
        }
    }

}
