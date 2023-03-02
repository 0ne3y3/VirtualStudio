// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VirtualStudioEditor : ModuleRules
{
	public VirtualStudioEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[] {
				"VirtualStudioEditor"
			}
		);

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EditorFramework", "VirtualStudio", "Characters", "UMGEditor", "EditorScriptingUtilities" });

		PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd", "UMG", "Blutility" });
        // "Slate", "SlateCore" 

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
