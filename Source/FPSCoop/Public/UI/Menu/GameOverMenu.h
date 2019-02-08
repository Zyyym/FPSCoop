// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/InGameMenu.h"
#include "GameOverMenu.generated.h"

/**
 * 
 */
UCLASS()
class FPSCOOP_API UGameOverMenu : public UInGameMenu
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = Menu)
		void SetStatus(bool bSucces);
};
