// Tencent is pleased to support the open source community by making UnLua available.
// 
// Copyright (C) 2019 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the MIT License (the "License"); 
// you may not use this file except in compliance with the License. You may obtain a copy of the License at
//
// http://opensource.org/licenses/MIT
//
// Unless required by applicable law or agreed to in writing, 
// software distributed under the License is distributed on an "AS IS" BASIS, 
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and limitations under the License.

#include "Misc/FileHelper.h"
#include "Containers/ArrayView.h"

#include "UnLuaEx.h"
#include "LuaCore.h"
#include "LuaDynamicBinding.h"



static int32 FFileHelper_LoadFileToArray(lua_State *L)
{
	bool bStatus = false;
	int32 NumParams = lua_gettop(L);
	if (NumParams != 1)
	{
		UNLUA_LOGERROR(L, LogUnLua, Log, TEXT("%s: Invalid parameters!"), ANSI_TO_TCHAR(__FUNCTION__));
		return 0;
	}
	else
	{
		FString FileName = luaL_tolstring(L, 1, nullptr);
		if (!FileName.IsEmpty())
		{
			if (!FPaths::FileExists(FileName))
			{
				UNLUA_LOGERROR(L, LogUnLua, Log, TEXT("%s: %s is not exists!"), ANSI_TO_TCHAR(__FUNCTION__),*FileName);
			}
			else
			{
				TArray<uint8> result;
				if (FFileHelper::LoadFileToArray(result, *FileName, EFileRead::FILEREAD_NoFail | EFileRead::FILEREAD_Silent))
				{
					lua_pushlstring(L, (const char*)result.GetData(), result.Num());
					bStatus = true;
				}

			}

		}
	}
	
	return bStatus ? 1 : 0;
}
static int32 FFileHelper_LoadFileToString(lua_State *L)
{
	bool bStatus = false;
	int32 NumParams = lua_gettop(L);
	if (NumParams != 1)
	{
		UNLUA_LOGERROR(L, LogUnLua, Log, TEXT("%s: Invalid parameters!"), ANSI_TO_TCHAR(__FUNCTION__));
		return 0;
	}
	else
	{
		FString FileName = luaL_tolstring(L, 1, nullptr);
		if (!FileName.IsEmpty())
		{
			if (!FPaths::FileExists(FileName))
			{
				UNLUA_LOGERROR(L, LogUnLua, Log, TEXT("%s: %s is not exists!"), ANSI_TO_TCHAR(__FUNCTION__), *FileName);
			}
			else
			{
				FString result;
				if (FFileHelper::LoadFileToString(result, *FileName))
				{
					FTCHARToUTF8 UTF8CHAR_Ins(*result);
					lua_pushlstring(L, (ANSICHAR*)(UTF8CHAR_Ins.Get()), UTF8CHAR_Ins.Length());
					bStatus = true;
				}
			}

		}
	}

	return bStatus ? 1 : 0;
}

/*
	@Param1 -- data
	@Param2 -- filename
	@Param3 -- WriteMode(Append?)
*/
static int32 FFileHelper_SaveArrayToFile(lua_State *L)
{
	int32 NumParams = lua_gettop(L);

	bool bAppend = false;
	if (NumParams < 2)
	{
		UNLUA_LOGERROR(L, LogUnLua, Log, TEXT("%s: Invalid parameters!"), ANSI_TO_TCHAR(__FUNCTION__));
	}
	else
	{
		size_t size = 0;
		uint8* data = (uint8*)lua_tolstring(L, 1, &size);
		FString FileName = luaL_tolstring(L, 2, nullptr);

		if (NumParams == 3)
		{
			bAppend = lua_toboolean(L, 3);
		}
		

		if (!FileName.IsEmpty() && data)
		{
			uint32 Flags = EFileWrite::FILEWRITE_AllowRead | EFileWrite::FILEWRITE_EvenIfReadOnly | (bAppend ? EFileWrite::FILEWRITE_Append : 0);
			bool bSaveStatus = FFileHelper::SaveArrayToFile(TArrayView<const uint8>(data, size), *FileName, &IFileManager::Get(), Flags);
			lua_pushboolean(L, bSaveStatus);
		}
	}
	return 1;
}

static int32 FFileHelper_SaveStringToFile(lua_State *L)
{
	return FFileHelper_SaveArrayToFile(L);
}

static const luaL_Reg FFileHelperLib[] =
{
	{ "LoadFileToArray", FFileHelper_LoadFileToArray },
	{ "LoadFileToString", FFileHelper_LoadFileToString },
	{ "SaveStringToFile", FFileHelper_SaveStringToFile },
	{ "SaveArrayToFile", FFileHelper_SaveArrayToFile },
	{ nullptr, nullptr }
};

// Export Actor Component
BEGIN_EXPORT_CLASS(FFileHelper)
	ADD_LIB(FFileHelperLib)
END_EXPORT_CLASS()
IMPLEMENT_EXPORTED_CLASS(FFileHelper)

