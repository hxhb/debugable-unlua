// Fill out your copyright notice in the Description page of Project Settings.
#include "LibLuacrypt.h"

#include "clua.h"
#include "lsha1.c"
#include "lua-crypt.c"

IMPLEMENT_MODULE(FLibLuacryptModule, LibLuacrypt);

void FLibLuacryptModule::StartupModule()
{
	IModularFeatures::Get().RegisterModularFeature(LUA_LIB_FEATURE_NAME, this);
	
}

void FLibLuacryptModule::ShutdownModule()
{
	IModularFeatures::Get().UnregisterModularFeature(LUA_LIB_FEATURE_NAME, this);
}

void FLibLuacryptModule::SetupLuacrypt(lua_State* L)
{
	luaL_getsubtable(L, LUA_REGISTRYINDEX, LUA_PRELOAD_TABLE);
		 
	lua_pushcfunction(L, luaopen_crypt);
	lua_setfield(L, -2, "crypt");
}


void FLibLuacryptModule::RegisterLuaLib(lua_State* L)
{
	luaL_getsubtable(L, LUA_REGISTRYINDEX, LUA_PRELOAD_TABLE);
		 
	lua_pushcfunction(L, luaopen_crypt);
	lua_setfield(L, -2, "crypt");
}
void FLibLuacryptModule::UnRegisterLuaLib(lua_State* L)
{
	
}
FName FLibLuacryptModule::GetLibName()const
{
	return TEXT("LibLuacrypt");
}