// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MultiplayerGS.generated.h"

/**
 * 
 */
UCLASS()
class FPSCOOP_API AMultiplayerGS : public AGameStateBase
{
	GENERATED_BODY()

public:
	void UpdateMisionObjective();

	void LostMisionObjective();

	void GameOver(bool bSucces);
};
