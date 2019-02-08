// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiplayerGM.h"
#include "Game/MultiplayerGS.h"
#include "Engine/World.h"

void AMultiplayerGM::UpdateMisionStatus()
{
	AMultiplayerGS* MGS = GetGameState<AMultiplayerGS>();
	if (MGS)
	{
		MGS->UpdateMisionObjective();
	}
}


void AMultiplayerGM::PawnLostObjective()
{
	AMultiplayerGS* MGS = GetGameState<AMultiplayerGS>();
	if (MGS)
	{
		MGS->LostMisionObjective();
	}
}


void AMultiplayerGM::GameOver(bool bSucess)
{
	AMultiplayerGS* MGS = GetGameState<AMultiplayerGS>();
	if (MGS)
	{
		MGS->GameOver(bSucess);
	}
}

void AMultiplayerGM::RestartLevel()
{
	//GetWorld()->ServerTravel("/Game/Main/Levels/PlayMap");
}