// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPSCoopProjectile.h"
#include "ImpulseProjectile.generated.h"

/**
 * 
 */
UCLASS()
class FPSCOOP_API AImpulseProjectile : public AFPSCoopProjectile
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile)
		float ImpulsePower;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile)
		float ImpulseSize;

public:
	AImpulseProjectile();

	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	virtual void Kill() override;
};
