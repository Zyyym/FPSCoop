// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MultiplayerGM.generated.h"

/**
 * 
 */
UCLASS()
class FPSCOOP_API AMultiplayerGM : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	void UpdateMisionStatus();

	void PawnLostObjective();

	void GameOver(bool bSucess);

	void RestartLevel();
};
