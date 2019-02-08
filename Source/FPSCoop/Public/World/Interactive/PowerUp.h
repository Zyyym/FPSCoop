// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerUp.generated.h"

UCLASS()
class FPSCOOP_API APowerUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = Components)
		USceneComponent* Scene;

	UPROPERTY(VisibleDefaultsOnly, Category = Components)
		class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PowerUp)
		class  UParticleSystem* TakeEffect;

	UPROPERTY(VisibleDefaultsOnly, Category = Components)
		class UStaticMeshComponent* MeshComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PowerUp)
		FVector EffectScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PowerUp)
		class USoundBase* TakeSound;

	bool bFlyUp;

	UPROPERTY(ReplicatedUsing = OnRep_SetLocation)
		FVector_NetQuantize10 RelativeLocation;

	UFUNCTION()
		void OnRep_SetLocation();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor * OtherActor) override;

};
