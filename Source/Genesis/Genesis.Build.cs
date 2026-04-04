// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Genesis : ModuleRules
{
	public Genesis(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// Consolidated all necessary modules here
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
			"GameplayAbilities", // GAS Core
			"GameplayTags",      // Required for GAS
			"GameplayTasks"      // Required for GAS
		});

		// Removed the redundant second AddRange block for better practice

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
        
		// Removed GAS from PrivateDependencyModuleNames since they are already in Public.
		// This prevents the linker from doing double work.
		PrivateDependencyModuleNames.AddRange(new string[] { 
			"SlateCore" 
		});
	}
}