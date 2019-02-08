// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMenu.h"
#include "Game/MultiGI.h"
#include "Player/FPSPlayerCon.h"
#include "Game/MultiplayerGM.h"
#include "Kismet/GameplayStatics.h"

void UInGameMenu::ResumeLevel()
{
	GetOwningPlayer<AFPSPlayerCon>()->InGameMenuClosed();
}


void UInGameMenu::RestartLevel()
{
	//Cast<AMultiplayerGM>(UGameplayStatics::GetGameInstance(GetWorld()))->RestartLevel();
	UMultiGI* GI = Cast<UMultiGI>(GetWorld()->GetGameInstance());
	if (GI)
	{
		GI->RestartLevel();
	}
}


void UInGameMenu::BackToMenu()
{
	UMultiGI* GI = Cast<UMultiGI>(GetWorld()->GetGameInstance());
	if (GI)
	{
		GI->BackToMenu();
	}
}


void UInGameMenu::QuitTheGame()
{
	UMultiGI* GI = Cast<UMultiGI>(GetWorld()->GetGameInstance());
	if (GI)
	{
		GI->QuitTheGame();
	}
}