// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/Interactive/PowerUp.h"
#include "AmmoGiver.generated.h"

class UAmmoType;
UCLASS()
class FPSCOOP_API AAmmoGiver : public APowerUp
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Regeneration")
		TSubclassOf<UAmmoType> AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Regeneration")
		int32 Amount;

public:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
