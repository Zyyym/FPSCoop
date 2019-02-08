// Fill out your copyright notice in the Description page of Project Settings.

#include "SessionResultWidget.h"
#include "Components/TextBlock.h"
#include "Game/MultiGI.h"

void USessionResultWidget::SetResultWidget(FBlueprintSessionResult NewSession, const FString& SessionName, const int32 Players, const int32 MaxPlayers)
{
	Session = NewSession;
	if (SessionNameText)
		SessionNameText->SetText(FText::FromString(SessionName));
	FString PlayersText = FString::FromInt(Players) + FString("/") + FString::FromInt(MaxPlayers);
	if (SessionPlayersText)
		SessionPlayersText->SetText(FText::FromString(PlayersText));
}


FReply USessionResultWidget::NativeOnMouseButtonDoubleClick(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	if (Session.OnlineResult.IsValid())
	{
		UMultiGI* MyGI = Cast<UMultiGI>(GetWorld()->GetGameInstance());
		if (MyGI)
			MyGI->JoinChoosenSession(Session);
	}
	return FReply::Handled();
}