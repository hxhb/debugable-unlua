## What is this?
[UnLua](https://github.com/Tencent/UnLua/)是Tencent开源的一个在UE中集成Lua的插件。
本仓库在其基础上修改(基础版本为[b33c540](https://github.com/Tencent/UnLua/commit/b33c540f56ce4e61fd7e44ee9297506f133aa3db)：

- 源码集成`Luasocket`
- 源码集成`LuaPanda 3.1.0`
- 源码集成`Lpeg`
- 源码集成`Sproto`
- 源码集成`Luacrypt`
- 源码集成[lua-protobuf#392ace3](https://github.com/starwing/lua-protobuf/commit/392ace3abd4a46e5c386e52d2d16c9aade52e46d)
- 集成`serpent`
- 统一了unlua加载C Lua模块的方式，只需要继承`FLuaLibFeature`按照接口实现即可。
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