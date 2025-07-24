// Copyright Epic Games, Inc. All Rights Reserved.

using System.Collections.Generic;
using UnrealBuildTool;

public class ConspiracyTheoryGame : ModuleRules
{
    public ConspiracyTheoryGame(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "AIModule",
            "NavigationSystem",
            "StateTreeModule",
            "GameplayStateTreeModule",
            "UMG",
            "Niagara"
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });

        PublicIncludePaths.AddRange(new string[]
        {
            "ConspiracyTheoryGame/Public/Player",
            "ConspiracyTheoryGame/Public/Components",
            "ConspiracyTheoryGame/Public/Interfaces",
            "ConspiracyTheoryGame/Public/Pickups",
            "ConspiracyTheoryGame/Public/UI",
            "ConspiracyTheoryGame/Public/MainMenu",
            "ConspiracyTheoryGame/Public/MainMenu/UI",
            "ConspiracyTheoryGame/Public/Enemy",
            "ConspiracyTheoryGame/Public/Boss",
            "ConspiracyTheoryGame/Public/Boss/RhythmMechanics",
            "ConspiracyTheoryGame/Public/Boss/UI",
            "ConspiracyTheoryGame/Public/Environment",
            "ConspiracyTheoryGame/Public/Sound"
        });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
