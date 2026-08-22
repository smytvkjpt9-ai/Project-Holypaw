using UnrealBuildTool;

public class HolypawEditorTarget : TargetRules
{
	public HolypawEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Oldest;
		ExtraModuleNames.Add("Holypaw");
	}
}
