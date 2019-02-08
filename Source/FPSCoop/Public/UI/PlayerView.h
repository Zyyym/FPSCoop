// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerView.generated.h"

class UWeaponsBar;
class AWeapon;
UCLASS()
class FPSCOOP_API UPlayerView : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(BlueprintReadWrite, Category = Widget)
		class UHealthBar* HealthBar;

	UPROPERTY(BlueprintReadWrite, Category = Widget)
		UWeaponsBar* WeaponBar;

public:
	void RefreshHealthBar();

	void AssingHealthComp(class UHealthComponent* NewComp);

	void RefreshWeaponBar(TArray<AWeapon*> Weapons);

	UFUNCTION(BlueprintImplementableEvent, Category = Mission)
		void UdpateObjectiveText(bool bHaveObjective);
};
