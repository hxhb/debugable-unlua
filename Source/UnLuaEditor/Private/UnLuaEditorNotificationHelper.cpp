// Fill out your copyright notice in the Description page of Project Settings.


#include "UnLuaEditorNotificationHelper.h"

// engine header 
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"

void UUnLuaEditorNotificationHelper::CreateSaveFileNotify(const FText& InMsg, const FString& InSavedFile, SNotificationItem::ECompletionState InState)
{
	auto Message = InMsg;
	FNotificationInfo Info(Message);
	Info.bFireAndForget = true;
	Info.ExpireDuration = 5.0f;
	Info.bUseSuccessFailIcons = false;
	Info.bUseLargeFont = false;

	if (!InSavedFile.IsEmpty())
	{
		const FString HyperLinkText = InSavedFile;
		Info.Hyperlink = FSimpleDelegate::CreateStatic(
			[](FString SourceFilePath)
		{
			FPlatformProcess::ExploreFolder(*SourceFilePath);
		},
			HyperLinkText
			);
		Info.HyperlinkText = FText::FromString(HyperLinkText);
	}
	
	FSlateNotificationManager::Get().AddNotification(Info)->SetCompletionState(InState);
}