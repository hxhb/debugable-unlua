// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

struct lua_State;

class LIBLUASOCKET_API FLibLuasocketModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void SetupLuasocket(lua_State* L);

	static inline FLibLuasocketModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FLibLuasocketModule>("LibLuasocket");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("LibLuasocket");
	}

protected:
	static int OpenLuaSocketFtp(lua_State* L);
	static int OpenLuaSocketHeaders(lua_State* L);
	static int OpenLuaSocketHttp(lua_State* L);
	static int OpenLuaSocketLtn12(lua_State* L);
	static int OpenLuaSocketMbox(lua_State* L);
	static int OpenLuaSocketMime(lua_State* L);
	static int OpenLuaSocketSmtp(lua_State* L);
	static int OpenLuaSocketSocket(lua_State* L);
	static int OpenLuaSocketTp(lua_State* L);
	static int OpenLuaSocketUrl(lua_State* L);
};
