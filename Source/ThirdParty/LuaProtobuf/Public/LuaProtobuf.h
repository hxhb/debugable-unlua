// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "LuaLibFeature.h"
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

struct lua_State;

class LUAPROTOBUF_API FLuaProtobuf : public FLuaLibFeature
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void SetupLuaProtobuf(lua_State* L);

	static inline FLuaProtobuf& Get()
	{
		return FModuleManager::LoadModuleChecked<FLuaProtobuf>("LuaProtobuf");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("LuaProtobuf");
	}

	virtual void RegisterLuaLib(lua_State* L);
	virtual void UnRegisterLuaLib(lua_State* L);
	virtual FName GetLibName()const;

protected:
	static int OpenLuaProtobuf(lua_State* L);
};
