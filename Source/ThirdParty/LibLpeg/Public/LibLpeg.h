// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

struct lua_State;

class LIBLPEG_API FLibLpegModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void SetupLibLpeg(lua_State* L);

	static inline FLibLpegModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FLibLpegModule>("LibLpeg");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("LibLpeg");
	}

protected:

};
