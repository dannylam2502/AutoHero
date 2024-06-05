// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AutoHero : ModuleRules
{
	public AutoHero(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		// Add this line to use C++20
		CppStandard = CppStandardVersion.Cpp20;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput", "GameplayAbilities", "GameplayTags", "GameplayTasks" });
        PrivateDependencyModuleNames.AddRange(new string[] {"NavigationSystem"});
	}
}
