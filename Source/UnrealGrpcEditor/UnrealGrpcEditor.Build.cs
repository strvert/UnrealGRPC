using UnrealBuildTool;

public class UnrealGrpcEditor : ModuleRules
{
	public UnrealGrpcEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"ToolMenus",
				"ToolWidgets",
				"PluginUtils",
				"Projects"
			}
		);
	}
}