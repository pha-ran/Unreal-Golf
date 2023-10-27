// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Week7Test : ModuleRules
{
	public Week7Test(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput",
															"HTTP", "Json"});
	}
}
