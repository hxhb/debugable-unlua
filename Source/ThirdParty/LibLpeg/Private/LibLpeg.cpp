// Fill out your copyright notice in the Description page of Project Settings.
#include "LibLpeg.h"

#include "lua.hpp"

extern "C" {

#include "lpcap.h"
#include "lpcode.h"
#include "lpprint.h"
#include "lptree.h"
#include "lptypes.h"
#include "lpvm.h"

}

IMPLEMENT_MODULE(FLibLpegModule, LibLpeg);

void FLibLpegModule::StartupModule()
{

}

void FLibLpegModule::ShutdownModule()
{

}

void FLibLpegModule::SetupLibLpeg(lua_State* L)
{
	//luaL_getsubtable(L, LUA_REGISTRYINDEX, LUA_PRELOAD_TABLE);
	//	 
	//lua_pushcfunction(L, luaopen_lpeg);
	//lua_setfield(L, -2, "lpeg");
	luaL_requiref(L, "lpeg", luaopen_lpeg , 1);
}

