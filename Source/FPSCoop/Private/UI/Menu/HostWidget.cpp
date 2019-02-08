// Fill out your copyright notice in the Description page of Project Settings.

#include "HostWidget.h"
#include "Game/MultiGI.h"

void UHostWidget::HostGame(const FText& ServerName, int32 GameMaxPlayers, bool bLan)
{
	UMultiGI* GI = Cast<UMultiGI>(GetWorld()->GetGameInstance());
	if (GI)
		GI->CreateLobby(ServerName, GameMaxPlayers, bLan);
}