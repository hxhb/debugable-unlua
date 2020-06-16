// Fill out your copyright notice in the Description page of Project Settings.

#include "LuaProtobuf.h"
#include "clua.h"
#include "pb.h"
#include "serpent_lua.h"
#include "protoc_lua.h"

IMPLEMENT_MODULE(FLuaProtobuf, LuaProtobuf);

void FLuaProtobuf::StartupModule()
{
	IModularFeatures::Get().RegisterModularFeature(LUA_LIB_FEATURE_NAME, this);
}

void FLuaProtobuf::ShutdownModule()
{
	IModularFeatures::Get().UnregisterModularFeature(LUA_LIB_FEATURE_NAME, this);
}
int OpenProtoc(lua_State* L)
{
	luaL_dostring(L, (const char*)protoc_lua_data);
	return 1;
}
int OpenSerpent(lua_State* L)
{
	luaL_dostring(L, (const char*)serpent_lua_data);
	return 1;
}

void FLuaProtobuf::SetupLuaProtobuf(struct lua_State* L)
{
	luaL_requiref(L, "pb", luaopen_pb, 0);
	luaL_requiref(L, "pb.slice", luaopen_pb_slice, 0);
	luaL_requiref(L, "pb.buffer", luaopen_pb_buffer, 0);
	luaL_requiref(L, "pb.conv", luaopen_pb_conv, 0);
	luaL_requiref(L, "protoc", &::OpenProtoc,1);
	luaL_requiref(L, "serpent", &::OpenSerpent,1);
}

void FLuaProtobuf::RegisterLuaLib(lua_State* L)
{
	FLuaProtobuf::SetupLuaProtobuf(L);
}
void FLuaProtobuf::UnRegisterLuaLib(lua_State* L)
{
	
}
FName FLuaProtobuf::GetLibName()const
{
	return TEXT("LuaProtobuf");
}