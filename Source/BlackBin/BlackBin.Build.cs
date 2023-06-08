// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BlackBin : ModuleRules
{
	public BlackBin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
<<<<<<< HEAD
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Niagara", "LevelSequence", "HeadMountedDisplay", "EnhancedInput", "UMG" });
=======
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Niagara", "LevelSequence", "EnhancedInput", "HeadMountedDisplay" });
>>>>>>> d7417eea52c41f32455bc48a5408e341714e3ff0

		PrivateDependencyModuleNames.AddRange(new string[] {"MovieScene"  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
