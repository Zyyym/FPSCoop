// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Menu/SubMenu.h"
#include "FindGameWidget.generated.h"

class USessionResultWidget;
UCLASS()
class FPSCOOP_API UFindGameWidget : public USubMenu
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Multiplayer)
		void LookForSessions();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = Multiplayer)
		void AddSesionResult(USessionResultWidget* SesionWidget);
};
