// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "LuaLibInterface.h"
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class LUALIBFEATURE_API FLuaLibFeature : public IModuleInterface,public ILuaLibInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual void RegisterLuaLib(lua_State* L);
	virtual void UnRegisterLuaLib(lua_State* L);
	virtual FName GetLibName()const;
};
