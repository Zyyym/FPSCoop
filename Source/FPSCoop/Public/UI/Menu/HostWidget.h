// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Menu/SubMenu.h"
#include "HostWidget.generated.h"

/**
 * 
 */
UCLASS()
class FPSCOOP_API UHostWidget : public USubMenu
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable, Category = Multiplayer)
		void HostGame(const FText& ServerName, int32 GameMaxPlayers, bool bLan);
};
