// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class LibLuasocket : ModuleRules
{
	public LibLuasocket(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core" ,"LuaLibFeature"});

		PrivateDependencyModuleNames.AddRange(new string[] { "Lua" });

		bEnableShadowVariableWarnings = false;
		bEnableUndefinedIdentifierWarnings = false;

		PublicDefinitions.Add("WITH_LIBLUASOCKET=1");

		if (Target.Configuration == UnrealTargetConfiguration.Debug ||
			Target.Configuration == UnrealTargetConfiguration.DebugGame)
		{
			PrivateDefinitions.Add("LUASOCKET_DEBUG");
		}
	}
}
