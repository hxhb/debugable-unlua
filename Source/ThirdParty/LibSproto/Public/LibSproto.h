// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "LuaLibFeature.h"
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

struct lua_State;

class LIBSPROTO_API FLibSprotoModule : public FLuaLibFeature
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void SetupLibSproto(lua_State* L);

	static inline FLibSprotoModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FLibSprotoModule>("LibSproto");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("LibSproto");
	}
	virtual void RegisterLuaLib(lua_State* L);
	virtual void UnRegisterLuaLib(lua_State* L);
	virtual FName GetLibName()const;
protected:

};
