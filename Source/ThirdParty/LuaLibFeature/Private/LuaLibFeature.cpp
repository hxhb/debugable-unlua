// Fill out your copyright notice in the Description page of Project Settings.

#include "LuaLibFeature.h"

IMPLEMENT_MODULE(FLuaLibFeature, LuaLibFeature);

void FLuaLibFeature::StartupModule()
{

}

void FLuaLibFeature::ShutdownModule()
{

}

void FLuaLibFeature::RegisterLuaLib(lua_State* L)
{
    
}
void FLuaLibFeature::UnRegisterLuaLib(lua_State* L)
{
    
}
FName FLuaLibFeature::GetLibName()const
{
   return  TEXT("");
}