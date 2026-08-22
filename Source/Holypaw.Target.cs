using UnrealBuildTool;

public class HolypawTarget : TargetRules
{
	public HolypawTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		// Latest always exists on 5.8. Oldest is the most relaxed IWYU the
		// installed engine still ships — pinning Unreal5_4/V5 can fail UBT
		// with CS0117 and the editor only says "missing module".
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Oldest;
		ExtraModuleNames.Add("Holypaw");
	}
}
