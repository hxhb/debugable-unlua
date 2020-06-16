// Fill out your copyright notice in the Description page of Project Settings.

#include "LibLuasocket.h"

#include "clua.h"

extern "C" {
#include "luasocket.h"
#include "mime.h"
}

IMPLEMENT_MODULE(FLibLuasocketModule, LibLuasocket);

void FLibLuasocketModule::StartupModule()
{
	IModularFeatures::Get().RegisterModularFeature(LUA_LIB_FEATURE_NAME, this);
	
}

void FLibLuasocketModule::ShutdownModule()
{
	IModularFeatures::Get().UnregisterModularFeature(LUA_LIB_FEATURE_NAME, this);
}

void FLibLuasocketModule::RegisterLuaLib(lua_State* L)
{
	FLibLuasocketModule::SetupLuasocket(L);
}
void FLibLuasocketModule::UnRegisterLuaLib(lua_State* L)
{
	
}
FName FLibLuasocketModule::GetLibName()const
{
	return TEXT("LibLuasocket");
}

void FLibLuasocketModule::SetupLuasocket(struct lua_State* L)
{
// 	luaL_getsubtable(L, LUA_REGISTRYINDEX, LUA_PRELOAD_TABLE);

	//lua_pushcfunction(L, luaopen_socket_core);
	//lua_setfield(L, -2, "socket.core");
	luaL_requiref(L, "socket.core", luaopen_socket_core, 1);

	//lua_pushcfunction(L, luaopen_mime_core);
	//lua_setfield(L, -2, "mime.core");
	luaL_requiref(L, "mime.core", luaopen_mime_core, 1);

	//lua_pushcfunction(L, &FLibLuasocketModule::OpenLuaSocketFtp);
	//lua_setfield(L, -2, "socket.ftp");
	luaL_requiref(L, "socket.ftp", &FLibLuasocketModule::OpenLuaSocketFtp, 1);

	/*lua_pushcfunction(L, &FLibLuasocketModule::OpenLuaSocketHeaders);
	lua_setfield(L, -2, "socket.headers");*/
	luaL_requiref(L, "socket.headers", &FLibLuasocketModule::OpenLuaSocketHeaders, 1);

	//lua_pushcfunction(L, &FLibLuasocketModule::OpenLuaSocketHttp);
	//lua_setfield(L, -2, "socket.http");
	luaL_requiref(L, "socket.http", &FLibLuasocketModule::OpenLuaSocketHttp, 1);

	/*lua_pushcfunction(L, &FLibLuasocketModule::OpenLuaSocketLtn12);
	lua_setfield(L, -2, "ltn12");*/
	luaL_requiref(L, "ltn12", &FLibLuasocketModule::OpenLuaSocketLtn12, 1);

	/*lua_pushcfunction(L, &FLibLuasocketModule::OpenLuaSocketMbox);
	lua_setfield(L, -2, "mbox");*/
	luaL_requiref(L, "mbox", &FLibLuasocketModule::OpenLuaSocketMbox, 1);

	//lua_pushcfunction(L, &FLibLuasocketModule::OpenLuaSocketMime);
	//lua_setfield(L, -2, "mime");
	luaL_requiref(L, "mime", &FLibLuasocketModule::OpenLuaSocketMime, 1);

	//lua_pushcfunction(L, &FLibLuasocketModule::OpenLuaSocketSmtp);
	//lua_setfield(L, -2, "socket.smtp");
	luaL_requiref(L, "socket.smtp", &FLibLuasocketModule::OpenLuaSocketSmtp, 1);

	/*lua_pushcfunction(L, &FLibLuasocketModule::OpenLuaSocketSocket);
	lua_setfield(L, -2, "socket");*/
	luaL_requiref(L, "socket", &FLibLuasocketModule::OpenLuaSocketSocket, 1);

	//lua_pushcfunction(L, &FLibLuasocketModule::OpenLuaSocketTp);
	//lua_setfield(L, -2, "socket.tp");
	luaL_requiref(L, "socket.tp", &FLibLuasocketModule::OpenLuaSocketTp, 1);

	//lua_pushcfunction(L, &FLibLuasocketModule::OpenLuaSocketUrl);
	//lua_setfield(L, -2, "socket.url");
	luaL_requiref(L, "socket.url", &FLibLuasocketModule::OpenLuaSocketUrl, 1);

	// lua_pop(L, 1);
}

int FLibLuasocketModule::OpenLuaSocketFtp(lua_State* L)
{
	static const auto RawLua =
#include "ftp.lua.inc"

	luaL_dostring(L, RawLua);
	return 1;
}

int FLibLuasocketModule::OpenLuaSocketHeaders(lua_State* L)
{
	static const auto RawLua =
#include "headers.lua.inc"

	luaL_dostring(L, RawLua);
	return 1;
}

int FLibLuasocketModule::OpenLuaSocketHttp(lua_State* L)
{
	static const auto RawLua =
#include "http.lua.inc"

	luaL_dostring(L, RawLua);
	return 1;
}

int FLibLuasocketModule::OpenLuaSocketLtn12(lua_State* L)
{
	static const auto RawLua =
#include "ltn12.lua.inc"

	luaL_dostring(L, RawLua);
	return 1;
}

int FLibLuasocketModule::OpenLuaSocketMbox(lua_State* L)
{
	static const auto RawLua =
#include "mbox.lua.inc"

	luaL_dostring(L, RawLua);
	return 1;
}

int FLibLuasocketModule::OpenLuaSocketMime(lua_State* L)
{
	static const auto RawLua =
#include "mime.lua.inc"

	luaL_dostring(L, RawLua);
	return 1;
}

int FLibLuasocketModule::OpenLuaSocketSmtp(lua_State* L)
{
	static const auto RawLua =
#include "smtp.lua.inc"

	luaL_dostring(L, RawLua);
	return 1;
}

int FLibLuasocketModule::OpenLuaSocketSocket(lua_State* L)
{
	static const auto RawLua =
#include "socket.lua.inc"

	luaL_dostring(L, RawLua);
	return 1;
}

int FLibLuasocketModule::OpenLuaSocketTp(lua_State* L)
{
	static const auto RawLua =
#include "tp.lua.inc"

	luaL_dostring(L, RawLua);
	return 1;
}

int FLibLuasocketModule::OpenLuaSocketUrl(lua_State* L)
{
	static const auto RawLua =
#include "url.lua.inc"

	luaL_dostring(L, RawLua);
	return 1;
}
