// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponWidget.generated.h"

class UTextBlock;
class AWeapon;
class UImage;
class UAmmoBar;
class UVerticalBox;
UCLASS()
class FPSCOOP_API UWeaponWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	AWeapon* Weapon;

	UTextBlock* AmmoText;

	UImage* Image;

	UVerticalBox* VerBox;

	UWidgetAnimation* PrimaryAnim;

	UWidgetAnimation* MakeUsableAnim;

	UFUNCTION(BlueprintCallable, Category = "WeaponWidget")
		void SetDefaults(UTextBlock* NewAmmoText, UImage* NewImage, UVerticalBox* NewVerBox, UWidgetAnimation* NewPrimaryAnim, UWidgetAnimation* NewUsableAnim);

	UAmmoBar* MyAmmoBar;

	bool bIsSet;

	UFUNCTION()
		void ForceRefresh();

public:
	void SetWeapon(AWeapon* NewWeapon);

	void Refresh();

	void MakePrimary();

	void MakeSecondary();

	void StartMakeUsable(float Time);
};
