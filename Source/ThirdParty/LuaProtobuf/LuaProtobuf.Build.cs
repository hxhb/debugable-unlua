// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class LuaProtobuf : ModuleRules
{
	public LuaProtobuf(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core","LuaLibFeature" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Lua" });

		bEnableShadowVariableWarnings = false;
		bEnableUndefinedIdentifierWarnings = false;

        PublicDefinitions.Add("PB_IMPLEMENTATION");
	}
}
