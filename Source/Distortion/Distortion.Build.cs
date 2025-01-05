// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Distortion : ModuleRules
{
	public Distortion(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateIncludePaths.AddRange(new string[] {
            "Distortion"
        });

        PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput",
            "Niagara",
            "UMG",
            "ModelViewViewModel",
        });
    }
}
