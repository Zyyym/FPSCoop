// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TrackerBot.generated.h"


UENUM(BlueprintType)
enum EAIMode {
	AIM_Stand UMETA(DisplayName = "Stand"),
	AIM_Patrol UMETA(DisplayName = "Patrol"),
	AIM_Looking UMETA(DisplayName = "Looking"),
	AIM_Agro UMETA(DisplayName = "Agro")
};


struct FAlertLocation
{
	bool IsSet;
	FVector Location;

	FAlertLocation()
	{
		IsSet = false;
		Location = FVector();
	}

	FAlertLocation(FVector NewLocation)
	{
		IsSet = true;
		Location = NewLocation;
	}

	void SetLocation(FVector NewLocation)
	{
		IsSet = true;
		Location = NewLocation;
	}

	void Checked()
	{
		IsSet = false;
	}
};


class UPawnSensingComponent;
class UHealthComponent;
class USphereComponent;
class ATargetPoint;
UCLASS()
class FPSCOOP_API ATrackerBot : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATrackerBot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		UHealthComponent* HealthComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		USphereComponent* CloseDetectionSphere;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		USphereComponent* ActorDetectionSphere;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		UPawnSensingComponent* PawnSensing;
	
	void FollowTarget();

	UFUNCTION()
		void OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume);

	UPROPERTY(Replicated)
		TEnumAsByte<EAIMode> AIState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrol")
		TArray<ATargetPoint*> PatrolPoints;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Patrol")
		ATargetPoint* NextPatrolPoint;

	UPROPERTY(Replicated)
		FVector HomeLocation;

	AActor* Target;

	TArray<AActor*> TargetsInRange;

	FAlertLocation TargetLocation;

	FAlertLocation NextTargetLocation;

	FVector GetNextPathPoint();

	UPROPERTY(Replicated)
		FVector NextPathPoint;

	bool bIsPathSet;

	virtual void NotifyHit(class UPrimitiveComponent * MyComp, AActor * Other, class UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit) override;

	void Explode();

	UFUNCTION()
		void RefreshPath();

	FTimerHandle RefreshPathTimer;

	void SetNextPatrolPoint();

	UFUNCTION()
		void OnCDSBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnADSBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnADSEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
