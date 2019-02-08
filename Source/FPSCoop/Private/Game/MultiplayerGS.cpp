// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiplayerGS.h"
#include "Player/FPSPlayerCon.h"
#include "Engine/World.h"

void AMultiplayerGS::UpdateMisionObjective()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AFPSPlayerCon* Cont = Cast<AFPSPlayerCon>(It->Get());
		if (Cont)
		{
			Cont->ClientUpdateMisionStatus(true);
		}
	}
}


void AMultiplayerGS::LostMisionObjective()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AFPSPlayerCon* Cont = Cast<AFPSPlayerCon>(It->Get());
		if (Cont)
		{
			Cont->ClientUpdateMisionStatus(false);
		}
	}
}


void AMultiplayerGS::GameOver(bool bSucces)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AFPSPlayerCon* Cont = Cast<AFPSPlayerCon>(It->Get());
		if (Cont)
		{
			Cont->ShowGameOverWidget(bSucces);
		}
	}
}