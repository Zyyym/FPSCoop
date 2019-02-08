// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponsBar.generated.h"

class AWeapon;
class UHorizontalBox;
class UWeaponWidget;
UCLASS()
class FPSCOOP_API UWeaponsBar : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponsBar")
		TSubclassOf<UWeaponWidget> WeaponWidgetClass;

	UWeaponWidget* ActualWidget;

	TArray<UWeaponWidget*> WaeponWidgets;

	UPROPERTY(BlueprintReadWrite, Category = "WeaponsBar")
		UHorizontalBox* Bar;
public:
	void CreateBar(TArray<AWeapon*> Weapons);
};
