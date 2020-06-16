#include "LibFmt.h"
#include "clua.h"

IMPLEMENT_MODULE(FLibFmt, LibFmt);

extern "C" int luaopen_fmt(struct lua_State *L);

void FLibFmt::StartupModule()
{
	IModularFeatures::Get().RegisterModularFeature(LUA_LIB_FEATURE_NAME, this);
}

void FLibFmt::ShutdownModule()
{
	IModularFeatures::Get().UnregisterModularFeature(LUA_LIB_FEATURE_NAME, this);
}

void FLibFmt::RegisterLuaLib(lua_State* L)
{
	luaL_requiref(L, "fmt", luaopen_fmt, 1);
}
void FLibFmt::UnRegisterLuaLib(lua_State* L)
{
    
}
FName FLibFmt::GetLibName()const
{
   return  TEXT("LibFmt");
}