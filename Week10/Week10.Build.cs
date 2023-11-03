// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Week10 : ModuleRules
{
	public Week10(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput",
                                                            "HTTP", "Json"});
	}
}
