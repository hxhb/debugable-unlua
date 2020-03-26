// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

struct lua_State;

class LUAPANDA_API FLuaPanda : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void SetupLuaPanda(lua_State* L);

	static inline FLuaPanda& Get()
	{
		return FModuleManager::LoadModuleChecked<FLuaPanda>("LuaPanda");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("LuaPanda");
	}

protected:
	static int OpenLuaPanda(lua_State* L);
};
