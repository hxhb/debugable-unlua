## What is this?
[UnLua](https://github.com/Tencent/UnLua/)是Tencent开源的一个在UE中集成Lua的插件。
本仓库在其基础上修改(基础版本为[bfabb2b](https://github.com/Tencent/UnLua/commit/bfabb2bd6cc9b934c677bef03a19ece8c7f7d700)：

- 源码集成`Luasocket`
- 源码集成`LuaPanda 3.1.0`
- 源码集成`Lpeg`
- 源码集成`Sproto`
- 源码集成`Luacrypt`
- 增加编辑器生成lua文件的提示、检测蓝图有没有继承`UnLuaInterface`接口。

## LuaPanda调试
首先下载VSC之后安装`Luapanda`插件，然后在项目的Lua文件中添加：

```lua
if WITH_LUA_DEBUG then
    require "LuaPanda"
    LuaPanda.start("127.0.0.1",8818)
end
```
可以使用`WITH_LUA_DEBUG`和`WITH_LUASOCKET`来检测调试环境。

并且我也根据不同的编译环境我添加了下列全局变量：

```lua
lua_pushboolean(L, !!UE_BUILD_SHIPPING);
lua_setglobal(L, "BUILD_SHIPPING");

lua_pushboolean(L, !!UE_BUILD_TEST);
lua_setglobal(L, "BUILD_TEST");

lua_pushboolean(L, !!UE_BUILD_DEVELOPMENT);
lua_setglobal(L, "BUILD_DEVELOPMENT");

lua_pushboolean(L, !!UE_BUILD_DEBUG);
lua_setglobal(L, "BUILD_DEBUG");
```
可以在项目的lua文件中使用。