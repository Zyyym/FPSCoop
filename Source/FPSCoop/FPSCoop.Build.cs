// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FPSCoop : ModuleRules
{
	public FPSCoop(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "OnlineSubsystem", "OnlineSubsystemUtils", "AIModule", "NavigationSystem", "GameplayTags" });

        DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

    }
}
