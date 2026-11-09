// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ShortPerson : ModuleRules
{
	public ShortPerson(ReadOnlyTargetRules Target) : base(Target)
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
			"SlateCore"
        });

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"ShortPerson",
			"ShortPerson/Variant_Platforming",
			"ShortPerson/Variant_Platforming/Animation",
			"ShortPerson/Variant_Combat",
			"ShortPerson/Variant_Combat/AI",
			"ShortPerson/Variant_Combat/Animation",
			"ShortPerson/Variant_Combat/Gameplay",
			"ShortPerson/Variant_Combat/Interfaces",
			"ShortPerson/Variant_Combat/UI",
			"ShortPerson/Variant_SideScrolling",
			"ShortPerson/Variant_SideScrolling/AI",
			"ShortPerson/Variant_SideScrolling/Gameplay",
			"ShortPerson/Variant_SideScrolling/Interfaces",
			"ShortPerson/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
