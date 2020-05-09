// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class LuaPanda : ModuleRules
{
	public LuaPanda(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core","LuaLibFeature" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Lua" });

		bEnableShadowVariableWarnings = false;
		bEnableUndefinedIdentifierWarnings = false;

		PublicDefinitions.Add("WITH_LUAPANDA=1");
		PrivateDefinitions.Add("USE_SOURCE_CODE");
	}
}
