using UnrealBuildTool;

public class ReactorKitCore : ModuleRules
{
    public ReactorKitCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "GameplayTags"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
        });
    }
}