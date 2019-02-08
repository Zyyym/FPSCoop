// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "STrackerBot.generated.h"


struct FKnownLocation 
{
	bool bIsSet;
	FVector Location;

	FKnownLocation()
	{
		bIsSet = false;
		Location = FVector();
	}

	FKnownLocation(FVector NewLocation)
	{
		bIsSet = true;
		Location = NewLocation;
	}

	void SetLocation(FVector NewLocation)
	{
		bIsSet = true;
		Location = NewLocation;
	}

	void Checked()
	{
		bIsSet = false;
	}

};

class USphereComponent;
class USoundCue;
class ATargetPoint;
class UHealthComponent;
UCLASS()
class FPSCOOP_API ASTrackerBot : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASTrackerBot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		USphereComponent* CloseSphereComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		USphereComponent* FarSphereComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		class UPawnSensingComponent* PawnSens;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		UHealthComponent* HealthComp;

	FVector GetNextPathPoint();

	// Next point in navigation path
	FVector NextPathPoint;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
		float MovementForce;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
		bool bUseVelocityChange;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
		float RequiredDistanceToTarget;

	UMaterialInstanceDynamic* MatInst;

	UFUNCTION()
		void Explode();

	UFUNCTION(NetMulticast, Reliable)
		void MulticastExplode();

	UFUNCTION()
		void OnDamaged(int32 Damage);

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
		UParticleSystem* ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
		float ExplosionRadius;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
		float ExplosionDamage;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
		USoundCue* SelfDestructSound;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
		USoundCue* ExplodeSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TrackerBot")
		TArray<ATargetPoint*> PatrolPoints;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TrackerBot")
		ATargetPoint* NextPatrolPoint;

	AActor* Target;

	TArray<AActor*> TargetsInRange;

	TArray<AActor*> TargetsToSee;

	FKnownLocation InterestingLocation;

	FKnownLocation SecInterestingLocation;

	bool IsTargetInView(AActor* Actor);

	void InterestingLocationDetected(FVector Location);

	UFUNCTION()
		void MaterialPulseLoopForward();

	UFUNCTION()
		void MaterialPulseLoopBackward();

	FLinearColor MaterialColor;

	UFUNCTION()
		void OnHearNoise(APawn *OtherActor, const FVector &Location, float Volume);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UFUNCTION()
		void OnCDSBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnADSBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnADSEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void NotifyHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit) override;


protected:
	FTimerHandle TimerHandle_RefreshPath;

	void RefreshPath();
};