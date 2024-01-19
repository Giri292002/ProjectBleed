// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectBleed : ModuleRules
{
	public ProjectBleed(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "PhysicsCore", "HeadMountedDisplay", "EnhancedInput", "Niagara", "UMG" });
		PrivateDependencyModuleNames.AddRange(new string[] {"FMODStudio", "Slate", "SlateCore"});
	}
}
