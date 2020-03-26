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
#include "Engine/World.h"


// Export Actor Component
BEGIN_EXPORT_REFLECTED_CLASS(UActorComponent)
	ADD_FUNCTION(RegisterComponent)
	ADD_FUNCTION(OnComponentCreated)
	ADD_FUNCTION(UnregisterComponent)
	ADD_CONST_FUNCTION_EX("IsRegistered",bool, IsRegistered)
	ADD_CONST_FUNCTION_EX("HasBeenCreated",bool, HasBeenCreated)
END_EXPORT_CLASS()
IMPLEMENT_EXPORTED_CLASS(UActorComponent)


// Export SceneComponent
DEFINE_TYPE(ECollisionChannel)

BEGIN_EXPORT_REFLECTED_CLASS(USceneComponent)
	ADD_FUNCTION(GetCollisionObjectType)
END_EXPORT_CLASS()
IMPLEMENT_EXPORTED_CLASS(USceneComponent)