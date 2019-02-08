// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Classes/GameplayTagAssetInterface.h"
#include "FPSCoopCharacter.generated.h"

class UInputComponent;
class UAmmoType;
class AWeapon;
UCLASS(config=Game)
class AFPSCoopCharacter : public ACharacter, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSwitchWeapon(bool bNext);

public:
	AFPSCoopCharacter();

protected:
	virtual void BeginPlay();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health)
		class UHealthComponent* HealthComp;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
		AWeapon* Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
		TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayTags")
		FGameplayTagContainer GameplayTags;

	UPROPERTY(ReplicatedUsing = OnRep_SetRot)
		FRotator NewRot;

	UFUNCTION()
		void OnRep_SetRot();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Objective)
		TSubclassOf<class AMissionObjective> ObjectiveClass;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	class USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	uint32 bUsingMotionControllers : 1;

	void AddProjectileAmmo(TSubclassOf<UAmmoType> AmmoClass, int32 Amount);

	TArray<AWeapon*> GetWeapons() { return Weapons; }

	void TakeObjective();

	UFUNCTION()
		void OnKilled();

	bool HaveObjective() { return bHaveObjective; }

protected:
	void OnFire();

	void OnStopFire();

	void MoveForward(float Val);

	void MoveRight(float Val);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	void NextWeapon();

	void PreviousWeapon();

	void NextAmmo();

	void PreviousAmmo();

	bool bHaveObjective;

	void OpenInGameMenu();

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = Projectile, SaveGame)
		TArray<TSubclassOf<AWeapon>> WeaponsClasses;

	UPROPERTY(Replicated, SaveGame)
		TArray<AWeapon*> Weapons;
	
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

public:
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }

	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override { TagContainer = GameplayTags; return; };
};

