// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class VirtualStudioEditorTarget : TargetRules
{
	public VirtualStudioEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
       // BuildEnvironment = TargetBuildEnvironment.Unique;
        ExtraModuleNames.Add("VirtualStudio");
		ExtraModuleNames.Add("VirtualStudioEditor");
		ExtraModuleNames.Add("Characters");
		ExtraModuleNames.Add("CharactersEditor");
	}
}
