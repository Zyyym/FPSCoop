// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPSCoopProjectile.h"
#include "DirectProjectile.generated.h"

/**
 * 
 */
UCLASS()
class FPSCOOP_API ADirectProjectile : public AFPSCoopProjectile
{
	GENERATED_BODY()


public:
	ADirectProjectile();

	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

};
