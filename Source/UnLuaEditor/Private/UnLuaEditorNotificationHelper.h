// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UnLuaEditorNotificationHelper.generated.h"

/**
 * 
 */
UCLASS()
class UUnLuaEditorNotificationHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static void CreateSaveFileNotify(const FText& InMsg, const FString& InSavedFile=TEXT(""), SNotificationItem::ECompletionState InState=SNotificationItem::ECompletionState::CS_Fail);
};
