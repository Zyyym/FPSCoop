// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapon/ScanWeapon.h"
#include "WeaponGraviter.generated.h"

/**
 * 
 */
UCLASS()
class FPSCOOP_API AWeaponGraviter : public AScanWeapon
{
	GENERATED_BODY()
private:

	UPrimitiveComponent* HoldedPrimitive;

	UFUNCTION(Server, Reliable, WithValidation)
		void SeverStartShooting();

	UFUNCTION(Server, Reliable, WithValidation)
		void SeverEndShooting();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Graviter")
		float Range;

public:
	AWeaponGraviter();

	virtual void StartShooting() override;

	virtual void EndShooting() override;
};
