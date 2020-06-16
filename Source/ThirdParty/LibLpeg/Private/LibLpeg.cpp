// Fill out your copyright notice in the Description page of Project Settings.
#include "LibLpeg.h"

#include "clua.h"

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
	IModularFeatures::Get().RegisterModularFeature(LUA_LIB_FEATURE_NAME, this);
}

void FLibLpegModule::ShutdownModule()
{
	IModularFeatures::Get().UnregisterModularFeature(LUA_LIB_FEATURE_NAME, this);
}

void FLibLpegModule::SetupLibLpeg(lua_State* L)
{
	luaL_requiref(L, "lpeg", luaopen_lpeg , 1);
}


void FLibLpegModule::RegisterLuaLib(lua_State* L)
{
	luaL_requiref(L, "lpeg", luaopen_lpeg , 1);
}
void FLibLpegModule::UnRegisterLuaLib(lua_State* L)
{
	
}
FName FLibLpegModule::GetLibName()const
{
	return TEXT("LibLpeg");
}