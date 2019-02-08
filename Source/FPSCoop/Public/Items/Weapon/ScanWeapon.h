// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapon/Weapon.h"
#include "ScanWeapon.generated.h"

/**
 * 
 */
UCLASS()
class FPSCOOP_API AScanWeapon : public AWeapon
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		int32 Ammo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		float RegenerationTime;

public:
	virtual int32 GetAmmo() const override;

	virtual float GetRegenerationTime() const override;
};
