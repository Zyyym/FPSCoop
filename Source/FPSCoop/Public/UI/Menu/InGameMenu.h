// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class FPSCOOP_API UInGameMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UFUNCTION(BlueprintCallable, Category = Menu)
		void ResumeLevel();

	UFUNCTION(BlueprintCallable, Category = Menu)
		void RestartLevel();

	UFUNCTION(BlueprintCallable, Category = Menu)
		void BackToMenu();

	UFUNCTION(BlueprintCallable, Category = Menu)
		void QuitTheGame();

};
