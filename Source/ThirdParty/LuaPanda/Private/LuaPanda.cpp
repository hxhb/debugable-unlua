// Fill out your copyright notice in the Description page of Project Settings.

#include "LuaPanda.h"

#include "lua.hpp"
#include "libpdebug.h"
#include "LuaPanda.inl"

IMPLEMENT_MODULE(FLuaPanda, LuaPanda);

void FLuaPanda::StartupModule()
{

}

void FLuaPanda::ShutdownModule()
{

}

void FLuaPanda::SetupLuaPanda(struct lua_State* L)
{
	pdebug_init(L);

	//luaL_getsubtable(L, LUA_REGISTRYINDEX, LUA_PRELOAD_TABLE);

	//lua_pushcfunction(L, &FLuaPanda::OpenLuaPanda);
	//lua_setfield(L, -2, "LuaPanda");

	luaL_requiref(L, "LuaPanda", &FLuaPanda::OpenLuaPanda,1);
	// lua_pop(L, 1);
}

int FLuaPanda::OpenLuaPanda(lua_State* L)
{

	UE_LOG(LogTemp, Log, TEXT("Call open lua panda"));
	luaL_dostring(L, (const char*)LuaPanda_lua_data);

	return 1;
}

