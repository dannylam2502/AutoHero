// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class AutoHeroServerTarget : TargetRules //Change this line according to the name of your project
{
	public AutoHeroServerTarget(TargetInfo Target) : base(Target) //Change this line according to the name of your project
	{
		Type = TargetType.Server;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("AutoHero"); //Change this line according to the name of your project
	}
}