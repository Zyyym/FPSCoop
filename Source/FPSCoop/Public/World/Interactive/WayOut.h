// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/Interactive/PowerUp.h"
#include "WayOut.generated.h"

/**
 * 
 */
UCLASS()
class FPSCOOP_API AWayOut : public APowerUp
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Particle)
		class UParticleSystem* Particle;
	
public:

	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
