// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "LuaLibFeature.h"
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class LIBFMT_API FLibFmt:public FLuaLibFeature
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual void RegisterLuaLib(lua_State* L)override;
	virtual void UnRegisterLuaLib(lua_State* L)override;
	virtual FName GetLibName()const override;
};
