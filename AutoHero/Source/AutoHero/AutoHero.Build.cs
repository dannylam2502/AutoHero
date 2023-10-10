// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AutoHero : ModuleRules
{
	public AutoHero(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput" });
        PrivateDependencyModuleNames.AddRange(new string[] {"NavigationSystem", "GameplayAbilities", "GameplayTags", "GameplayTasks"});
	}
}
