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

#include "UnLuaEx.h"
#include "LuaCore.h"
#include "LuaDynamicBinding.h"
#include "UObject/PrimaryAssetId.h"

static int32 FPrimaryAssetId_tostring(lua_State *L)
{
	int32 NumParams = lua_gettop(L);
	if (NumParams != 1)
	{
		UE_LOG(LogUnLua, Log, TEXT("Invalid parameters for __tostring!"));
		return 0;
	}
	FPrimaryAssetId *AssetId = (FPrimaryAssetId*)GetCppInstanceFast(L, 1);
	if (AssetId && AssetId->IsValid())
	{
		lua_pushstring(L, TCHAR_TO_ANSI(*AssetId->ToString()));
	}
	return 1;
}


static const luaL_Reg FPrimaryAssetIdLib[] =
{
	{ "__tostring", FPrimaryAssetId_tostring },
	{ nullptr, nullptr }
};

// Export Actor Component
BEGIN_EXPORT_CLASS(FPrimaryAssetId,const FString&)
	ADD_FUNCTION_EX("IsValid", bool, IsValid)
	ADD_FUNCTION_EX("ToString", FString, ToString)
	ADD_STATIC_FUNCTION_EX("FromString", FPrimaryAssetId, FromString,const FString&)
	ADD_LIB(FPrimaryAssetIdLib)
END_EXPORT_CLASS()
IMPLEMENT_EXPORTED_CLASS(FPrimaryAssetId)

