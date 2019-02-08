// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Slate/SlateBrushAsset.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum EShotType
{
	ST_Single UMETA(DisplayName = "Single"),
	ST_Burst UMETA(DisplayName = "Burst"),
	ST_Auto UMETA(DisplayName = "Auto"),
	ST_Hold UMETA(DisplayName = "Hold"),

};

class UWeaponWidget;
UCLASS()
class FPSCOOP_API AWeapon : public AActor
{
	GENERATED_BODY()
private:
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerStartShoting();

	UFUNCTION(Client, Reliable)
		void ClientShowRegeneration(float Time);

	UFUNCTION(Client, Reliable)
		void ClientUpdateWidget(bool bPrimary);
	
protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		TEnumAsByte<EShotType> ShotMode;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		FSlateBrush Image;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* FP_Gun;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* FP_MuzzleLocation;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USceneComponent* Scene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		class USoundBase* FireSound;

	UPROPERTY(Replicated, SaveGame)
		APawn* MyPawn;

	UWeaponWidget* Widget;

	UFUNCTION(NetMulticast, Unreliable)
		void MulticastPlayShotSound();

	bool bCanShot;

	UFUNCTION()
		void Regenerate() { bCanShot = true; }
	
public:
	virtual void BeginPlay() override;

	AWeapon();

	void WeaponSwapedPrimary();

	void WeaponSwapedSecondary();

	virtual void StartShooting();

	virtual void EndShooting();

	virtual void Shot();

	void SetMyPawn(APawn* MyNewPawn);

	void SetWidget(UWeaponWidget* NewWidget);

	virtual float GetRegenerationTime() const;

	FSlateBrush GetImage() const { return Image; };

	virtual int32 GetAmmo() const;
		
};
