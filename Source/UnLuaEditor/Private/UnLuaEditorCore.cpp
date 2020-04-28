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

#include "UnLuaPrivate.h"
#include "UnLuaInterface.h"
#include "UnLuaEditorNotificationHelper.h"

// engine header
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/FileHelper.h"
#include "Engine/Blueprint.h"
#include "Blueprint/UserWidget.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "UnLuaEditorCore"

// create Lua template file for the selected blueprint
bool CreateLuaTemplateFile(UBlueprint *Blueprint, bool bShowFileNotifiction)
{
    if (Blueprint)
    {
        UClass *Class = Blueprint->GeneratedClass;
		
		UClass* GeneratedClass = Blueprint->GeneratedClass;

		bool bImplUnLuaInterface = UKismetSystemLibrary::DoesImplementInterface(GeneratedClass->ClassDefaultObject, UUnLuaInterface::StaticClass());

		if (!bImplUnLuaInterface)
		{
			UE_LOG(LogUnLua, Warning, TEXT("The Blueprint class is not directly add UnLuaInterface!"));
			auto Msg = LOCTEXT("BlueprintIsntInheritanceUnLua","The Blueprint class is not add UnLuaInterface!");
			UUnLuaEditorNotificationHelper::CreateSaveFileNotify(Msg, TEXT(""));
			return false;
		}
        FString ClassName = Class->GetName();
        FString OuterPath = Class->GetPathName();
        int32 LastIndex;
        if (OuterPath.FindLastChar('/', LastIndex))
        {
            OuterPath = OuterPath.Left(LastIndex + 1);
        }
        OuterPath = OuterPath.RightChop(6);         // ignore "/Game/"
        FString FileName = FString::Printf(TEXT("%s%s%s.lua"), *GLuaSrcFullPath, *OuterPath, *ClassName);
        if (FPaths::FileExists(FileName))
        {
            UE_LOG(LogUnLua, Warning, TEXT("Lua file (%s) is already existed!"), *ClassName);
			if (bShowFileNotifiction)
			{
				auto Msg = LOCTEXT("LuaFileIsAlreadtExisted", "Lua file is already existed!");
				UUnLuaEditorNotificationHelper::CreateSaveFileNotify(Msg, FileName,SNotificationItem::ECompletionState::CS_Success);
			}
            return false;
        }

        static FString ContentDir = IPluginManager::Get().FindPlugin(TEXT("UnLua"))->GetContentDir();

        FString TemplateName;
        if (Class->IsChildOf(AActor::StaticClass()))
        {
            // default BlueprintEvents for Actor
            TemplateName = ContentDir + TEXT("/ActorTemplate.lua");
        }
        else if (Class->IsChildOf(UUserWidget::StaticClass()))
        {
            // default BlueprintEvents for UserWidget (UMG)
            TemplateName = ContentDir + TEXT("/UserWidgetTemplate.lua");
        }
        else if (Class->IsChildOf(UAnimInstance::StaticClass()))
        {
            // default BlueprintEvents for AnimInstance (animation blueprint)
            TemplateName = ContentDir + TEXT("/AnimInstanceTemplate.lua");
        }
        else if (Class->IsChildOf(UActorComponent::StaticClass()))
        {
            // default BlueprintEvents for ActorComponent
            TemplateName = ContentDir + TEXT("/ActorComponentTemplate.lua");
        }
		else if (Class->IsChildOf(UObject::StaticClass()))
		{
			// default BlueprintEvents for ActorComponent
			TemplateName = ContentDir + TEXT("/ObjectTemplate.lua");
		}

        FString Content;
        FFileHelper::LoadFileToString(Content, *TemplateName);
        Content = Content.Replace(TEXT("TemplateName"), *ClassName);

        bool bSaveStatues = FFileHelper::SaveStringToFile(Content, *FileName);
		if (bShowFileNotifiction)
		{
			auto Msg = LOCTEXT("CreateLuaFile", "Lua file is Created!");
			UUnLuaEditorNotificationHelper::CreateSaveFileNotify(Msg, FileName,SNotificationItem::ECompletionState::CS_Success);
		}
		return bSaveStatues;
    }
    return false;
}

#undef LOCTEXT_NAMESPACE