// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AutoHero : ModuleRules
{
	public AutoHero(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateDependencyModuleNames.AddRange(new string[] { "PlayFab", "PlayFabCpp", "PlayFabCommon" });
        PrivateDependencyModuleNames.Add("OnlineSubsystem");

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput" });
    }
}
