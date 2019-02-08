// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

class UHealthComponent;
UCLASS()
class FPSCOOP_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

protected:
	UHealthComponent* HealthComp;

	int32 ActualHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Health)
		FSlateBrush HealthIcon;

	UPROPERTY(BlueprintReadWrite, Category = Health)
		class UHorizontalBox* Bar;
	
public:
	void RefreshBar();

	void AssingComp(UHealthComponent* NewHealth);
};
