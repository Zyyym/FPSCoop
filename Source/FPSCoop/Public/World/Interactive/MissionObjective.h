// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/Interactive/PowerUp.h"
#include "MissionObjective.generated.h"

/**
 * 
 */
UCLASS()
class FPSCOOP_API AMissionObjective : public APowerUp
{
	GENERATED_BODY()

	UPROPERTY(Replicated)
		bool bCanBeTaken;
	
	UFUNCTION()
		void StartBeAvaible();
public:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void BeginPlay() override;
};
