// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LWE_WOWTarget : TargetRules
{
    public LWE_WOWTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
        ExtraModuleNames.Add("LWE_WOW");
    }
}
