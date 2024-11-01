// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LWE_WOWEditorTarget : TargetRules
{
    public LWE_WOWEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
        ExtraModuleNames.Add("LWE_WOW");
    }
}
