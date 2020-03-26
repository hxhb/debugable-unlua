// Fill out your copyright notice in the Description page of Project Settings.
#include "LibSproto.h"

#include "lua.hpp"


extern "C" {
#include "sproto.h"
#include "msvcint.h"

}
#include "lsproto.c"
IMPLEMENT_MODULE(FLibSprotoModule, LibSproto);

void FLibSprotoModule::StartupModule()
{

}

void FLibSprotoModule::ShutdownModule()
{

}

void FLibSprotoModule::SetupLibSproto(lua_State* L)
{
	//luaL_getsubtable(L, LUA_REGISTRYINDEX, LUA_PRELOAD_TABLE);
		 
	//lua_pushcfunction(L, luaopen_sproto_core);
	// lua_setfield(L, -2, "sproto.core");
	luaL_requiref(L, "sproto.core", luaopen_sproto_core, 1);
}

