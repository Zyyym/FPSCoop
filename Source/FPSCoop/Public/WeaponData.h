// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Slate/SlateBrushAsset.h"
#include "WeaponData.generated.h"

class AFPSCoopProjectile;
UCLASS()
class FPSCOOP_API UWeaponData : public UDataAsset
{
	GENERATED_BODY()


protected:
	bool bCanShoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		float RegenerationTime;

	UFUNCTION()
		void Regenerate() { bCanShoot = true; };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		int32 MaxAmmo;

	int32 AddAmmo(int32 AddedAmmo);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		FSlateBrush Image;

public:
	UWeaponData();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		TSubclassOf<AFPSCoopProjectile> AmmoType;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Weapon", SaveGame)
		int32 Ammo;

	bool CanShoot() const { return bCanShoot; };

	bool Shot();
};
