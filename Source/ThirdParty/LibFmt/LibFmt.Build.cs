// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class LibFmt : ModuleRules
{
	public LibFmt(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core","LuaLibFeature" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Lua" });

        bEnableUndefinedIdentifierWarnings = false;
    }
}
