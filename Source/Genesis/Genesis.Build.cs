// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Genesis : ModuleRules
{
	public Genesis(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"GameplayAbilities",
			"EnhancedInput",
		});
		
		PublicDependencyModuleNames.AddRange(new string[] {
		"Core",
		"CoreUObject",
		"Engine",
		"InputCore",
		"EnhancedInput",
		"GameplayAbilities",
		"GameplayTags",
		"GameplayTasks"
		});

        PublicIncludePaths.AddRange(new string[] {
			"Genesis",
			"Genesis/Variant_Platforming",
			"Genesis/Variant_Platforming/Animation",
			"Genesis/Variant_Combat",
			"Genesis/Variant_Combat/AI",
			"Genesis/Variant_Combat/Animation",
			"Genesis/Variant_Combat/Gameplay",
			"Genesis/Variant_Combat/Interfaces",
			"Genesis/Variant_Combat/UI",
			"Genesis/Variant_SideScrolling",
			"Genesis/Variant_SideScrolling/AI",
			"Genesis/Variant_SideScrolling/Gameplay",
			"Genesis/Variant_SideScrolling/Interfaces",
			"Genesis/Variant_SideScrolling/UI"
		});
        
        PrivateDependencyModuleNames.AddRange(new string[] { "GameplayAbilities", "GameplayTags", "GameplayTasks"});
        
		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
