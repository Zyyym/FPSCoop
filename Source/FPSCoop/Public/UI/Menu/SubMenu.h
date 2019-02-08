// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SubMenu.generated.h"

/**
 * 
 */
UCLASS()
class FPSCOOP_API USubMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, Category = Menu)
		void BackToMenu();
};
