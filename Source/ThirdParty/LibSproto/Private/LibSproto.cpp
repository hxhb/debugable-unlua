// Fill out your copyright notice in the Description page of Project Settings.
#include "LibSproto.h"

#include "clua.h"


extern "C" {
#include "sproto.h"
#include "msvcint.h"

}
#include "lsproto.c"
IMPLEMENT_MODULE(FLibSprotoModule, LibSproto);

void FLibSprotoModule::StartupModule()
{
	IModularFeatures::Get().RegisterModularFeature(LUA_LIB_FEATURE_NAME, this);
	
}

void FLibSprotoModule::ShutdownModule()
{
	IModularFeatures::Get().UnregisterModularFeature(LUA_LIB_FEATURE_NAME, this);
}

void FLibSprotoModule::SetupLibSproto(lua_State* L)
{
	luaL_requiref(L, "sproto.core", luaopen_sproto_core, 1);
}


void FLibSprotoModule::RegisterLuaLib(lua_State* L)
{
	FLibSprotoModule::SetupLibSproto(L);
}
void FLibSprotoModule::UnRegisterLuaLib(lua_State* L)
{
	
}
FName FLibSprotoModule::GetLibName()const
{
	return TEXT("LibSproto");
}