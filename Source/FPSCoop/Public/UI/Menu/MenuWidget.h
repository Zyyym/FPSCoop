// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

class UMenuButton;
UCLASS()
class FPSCOOP_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UMenuButton* HostGameButton;

	UMenuButton* FindGameButton;

	UMenuButton* OptionsButton;

	UMenuButton* QuitGamaButton;
	
	UFUNCTION(BlueprintCallable, Category = Menu)
		void HostGame();

	UFUNCTION(BlueprintCallable, Category = Menu)
		void FindGames();

	UFUNCTION(BlueprintCallable, Category = Menu)
		void OpenOptions();

	UFUNCTION(BlueprintCallable, Category = Menu)
		void SetButtons(UMenuButton* NewHostGameButton, UMenuButton* NewFindGameButton, UMenuButton* NewOptionsButton, UMenuButton* NewQuitGamaButton);

public:
	virtual void NativeConstruct() override;
};
