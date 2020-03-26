// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

struct lua_State;

class LIBSPROTO_API FLibSprotoModule : public IModuleInterface
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

protected:

};
