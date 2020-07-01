## What is this?
[UnLua](https://github.com/Tencent/UnLua/)是Tencent开源的一个在UE中集成Lua的插件。
本仓库在其基础上修改(基础版本为[106fa9c](https://github.com/Tencent/UnLua/commit/106fa9c3a1a75b4af0e3e6b7e61b0c45609dd53d)，目的是打造一个开箱即用的UnLua，**可调试**和一些基础lua库、编辑器优化。

- 源码集成`Luasocket`
- 源码集成`LuaPanda 3.1.0`
- 源码集成`Lpeg`
- 源码集成`Sproto`
- 源码集成`Luacrypt`
- 源码集成[lua-protobuf#392ace3](https://github.com/starwing/lua-protobuf/commit/392ace3abd4a46e5c386e52d2d16c9aade52e46d)
- 集成`serpent`
- 源码集成[lua-fmt](https://github.com/starwing/lua-fmt)
- 统一了unlua加载C Lua模块的方式，只需要继承`FLuaLibFeature`按照接口实现即可。
- 增加编辑器生成lua文件的提示、检测蓝图有没有继承`UnLuaInterface`接口。

## LuaPanda调试
首先下载VSC之后安装`Luapanda`插件，然后在项目的Lua文件中添加：

```lua
if not BUILD_SHIPPING then
    require "LuaPanda"
    LuaPanda.start("127.0.0.1",8818)
end
```

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
