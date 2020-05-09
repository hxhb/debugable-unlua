#pragma once
#include "Features/IModularFeatures.h"
#include "CoreMinimal.h"

struct lua_State;
#define LUA_LIB_FEATURE_NAME "LuaLibFeature"

struct ILuaLibInterface:public IModularFeature
{
	virtual void RegisterLuaLib(lua_State* L)=0;
	virtual void UnRegisterLuaLib(lua_State* L)=0;
	virtual FName GetLibName()const =0 ;
	virtual FString GetLuaMacro()const
	{
		FString FullMacro = TEXT("WITH_")+GetLibName().ToString().ToUpper();
		return FullMacro;
	}
};