// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapon/Weapon.h"
#include "ProjectileWeapon.generated.h"

class UAmmoType;
class UAmmoBar;
UCLASS()
class FPSCOOP_API AProjectileWeapon : public AWeapon
{
	GENERATED_BODY()
private:
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerNextAmmo();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerPreviousAmmo();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerShot();

protected:

	UPROPERTY(Replicated, SaveGame)
		UAmmoType* Projectile;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile)
		TSubclassOf<UAmmoBar> AmmoBarClass;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = Projectile, SaveGame)
		TArray<TSubclassOf<UAmmoType>> AmmoClasses;

	UPROPERTY(Replicated, SaveGame)
		TArray<UAmmoType*> Ammos;


public:
	virtual void PostInitializeComponents() override;
	virtual bool ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags) override;

	virtual void BeginPlay() override;

	virtual void Shot() override;

	void NextAmmo();

	void PreviousAmmo();

	virtual float GetRegenerationTime() const override;

	UAmmoBar* GetAmmoBar();

	void AddAmmo(TSubclassOf<UAmmoType> AmmoClass, int32 Amount);

};
