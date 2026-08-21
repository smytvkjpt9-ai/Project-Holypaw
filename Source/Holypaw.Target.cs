using UnrealBuildTool;

public class HolypawTarget : TargetRules
{
	public HolypawTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("Holypaw");
	}
}
