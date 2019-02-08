// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AmmoWidget.generated.h"

class UTextBlock;
class UAmmoType;
class UImage;
UCLASS()
class FPSCOOP_API UAmmoWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UAmmoType* Ammo;

	UPROPERTY(BlueprintReadWrite, Category = "AmmoWidget")
		UTextBlock* AmmoText;

	UPROPERTY(BlueprintReadWrite, Category = "AmmoWidget")
		UImage* Image;

	UWidgetAnimation* PrimaryAnim;

	UFUNCTION(BlueprintCallable, Category = "WeaponWidget")
		void SetDefaults(UTextBlock* NewAmmoText, UImage* NewImage, UWidgetAnimation* NewPrimaryAnim);

public:
	virtual void NativeConstruct() override;

	void SetAmmo(UAmmoType* NewAmmo);

	void Refresh();

	void MakePrimary();

	void MakeSecondary();
};
