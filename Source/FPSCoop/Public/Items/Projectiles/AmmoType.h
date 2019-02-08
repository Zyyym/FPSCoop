// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Slate/SlateBrushAsset.h"
#include "AmmoType.generated.h"

class AFPSCoopProjectile;
class UAmmoWidget;
UCLASS(Blueprintable)
class FPSCOOP_API UAmmoType : public UObject
{
	GENERATED_BODY()
	
protected:
	bool bCanShoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
		float RegenerationTime;

	UFUNCTION()
		void Regenerate() { bCanShoot = true; };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
		int32 MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
		FSlateBrush Image;

	UAmmoWidget* MyWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
		TSubclassOf<AFPSCoopProjectile> AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo", SaveGame, ReplicatedUsing = OnRep_SetAmmo)
		int32 Ammo;

	UPROPERTY(SaveGame, ReplicatedUsing = OnRep_SetIsPrimary)
		int8 IsPrimary;

	UFUNCTION()
		void OnRep_SetAmmo();

	UFUNCTION()
		void OnRep_SetIsPrimary();

public:
	UAmmoType(const FObjectInitializer & X);

	UPROPERTY(Replicated)
		uint32 bReplicatedFlag : 1;

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

	int32 AddAmmo(int32 Amount);

	bool CanShoot() const;

	virtual bool Shot();

	void SetWidget(UAmmoWidget* NewWidget);

	FSlateBrush GetImage() const;

	int32 GetAmmo() const;

	TSubclassOf<AFPSCoopProjectile> GetAmmoType() const;

	float GetRegenerationTime() const;

	void MakePrimary();

	void MakeSecondary();
};
