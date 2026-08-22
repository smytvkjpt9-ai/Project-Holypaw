using UnrealBuildTool;

public class Holypaw : ModuleRules
{
	public Holypaw(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// Holypaw uses folder-prefixed includes (Actors/, UI/, Cities/, …).
		// UE 5.8 Latest build settings omit legacy parent include paths, so without
		// the module root on the path you get C1083 on HolypawWorldBuilder.h, etc.
		PublicIncludePaths.Add(ModuleDirectory);
		PrivateIncludePaths.Add(ModuleDirectory);

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"UMG",
			"Slate",
			"SlateCore",
			"ProceduralMeshComponent"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"RenderCore",
			"RHI",
			"AudioMixer"
		});
	}
}
