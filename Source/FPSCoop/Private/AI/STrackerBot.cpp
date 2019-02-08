// Fill out your copyright notice in the Description page of Project Settings.

#include "STrackerBot.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "NavigationSystem/Public/NavigationPath.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "Components/HealthComponent.h"
#include "Sound/SoundCue.h"
#include "Engine/World.h"
#include "Public/TimerManager.h"
#include "Engine/TargetPoint.h"
#include "GameplayTags.h"
#include "Library/ActionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ASTrackerBot::ASTrackerBot()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCanEverAffectNavigation(false);
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetNotifyRigidBodyCollision(true);
	MeshComp->SetCollisionObjectType(ECC_Pawn);
	RootComponent = MeshComp;
	MeshComp->SetRelativeScale3D(FVector(0.15f, 0.15f, 0.15f));

	CloseSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("CloseSphereComp"));
	CloseSphereComp->SetSphereRadius(100);
	CloseSphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CloseSphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CloseSphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CloseSphereComp->SetupAttachment(RootComponent);
	CloseSphereComp->SetRelativeScale3D(FVector(6, 6, 6));
	CloseSphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASTrackerBot::OnCDSBeginOverlap);

	FarSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("FarSphereComp"));
	FarSphereComp->SetSphereRadius(800);
	FarSphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	FarSphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	FarSphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	FarSphereComp->SetupAttachment(RootComponent);
	FarSphereComp->SetRelativeScale3D(FVector(6, 6, 6));
	FarSphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASTrackerBot::OnADSBeginOverlap);
	FarSphereComp->OnComponentEndOverlap.AddDynamic(this, &ASTrackerBot::OnADSEndOverlap);

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	HealthComp->OnKilled.AddDynamic(this, &ASTrackerBot::Explode);
	HealthComp->OnDamageTaken.AddDynamic(this, &ASTrackerBot::OnDamaged);

	PawnSens = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSens"));
	PawnSens->bSeePawns = false;
	PawnSens->HearingThreshold = 1300;
	PawnSens->LOSHearingThreshold = 700;
	PawnSens->OnHearNoise.AddDynamic(this, &ASTrackerBot::OnHearNoise);

	bUseVelocityChange = true;
	MovementForce = 1000;
	RequiredDistanceToTarget = 100;

	ExplosionDamage = 2;
	ExplosionRadius = 400;

	MaterialColor = FLinearColor(0.05f, 0.2f, 0.07f);
}

// Called when the game starts or when spawned
void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();

	if (Role == ROLE_Authority)
	{
		NextPathPoint = GetNextPathPoint();

		NextPatrolPoint = PatrolPoints[0];
	}
}


void ASTrackerBot::OnHearNoise(APawn *OtherActor, const FVector &Location, float Volume)
{
	InterestingLocationDetected(Location);
}


FVector ASTrackerBot::GetNextPathPoint()
{
	UNavigationPath* NavPath;
	GetWorldTimerManager().ClearTimer(TimerHandle_RefreshPath);
	GetWorldTimerManager().SetTimer(TimerHandle_RefreshPath, this, &ASTrackerBot::RefreshPath, 5.f, false);

	AActor* BestTarget = nullptr;
	float NearestTargetDistance = FLT_MAX;

	for (AActor* Ac : TargetsInRange)
	{
		if (Ac == nullptr || !IsTargetInView(Ac))
		{
			continue;
		}

		float Distance = (Ac->GetActorLocation() - GetActorLocation()).Size();

		if (Distance < NearestTargetDistance)
		{
			BestTarget = Ac;
			NearestTargetDistance = Distance;
		}
	}

	if (BestTarget)
	{
		UNavigationPath* NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), BestTarget);
		InterestingLocation.SetLocation(BestTarget->GetActorLocation());
		if (NavPath && NavPath->PathPoints.Num() > 1)
		{
			return NavPath->PathPoints[1];
		}
	}


	if (InterestingLocation.bIsSet)
	{
		UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, GetActorLocation(), InterestingLocation.Location);

		if (NavPath && NavPath->PathPoints.Num() > 1)
		{
			return NavPath->PathPoints[1];
		}
	}


	if (SecInterestingLocation.bIsSet)
	{
		UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, GetActorLocation(), SecInterestingLocation.Location);
		InterestingLocation.SetLocation(SecInterestingLocation.Location);
		SecInterestingLocation.Checked();
		if (NavPath && NavPath->PathPoints.Num() > 1)
		{
			return NavPath->PathPoints[1];
		}
	}


	if (NextPatrolPoint)
	{
		NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), NextPatrolPoint);
		if (NavPath && NavPath->PathPoints.Num() > 1)
		{
			return NavPath->PathPoints[1];
		}
	}

	return GetActorLocation();
}


void ASTrackerBot::Explode()
{
	MulticastExplode();

	if (Role == ROLE_Authority)
	{
		TArray<AActor*> Ignore;
		Ignore.Add(this);

		UActionLibrary::AddRadialImpulseToComponents(GetActorLocation(), 200.f, 800.f, Ignore);

		UActionLibrary::AddRadialImpulseToPawns(GetActorLocation(), 180.f, 800.f, Ignore, 2);

		Destroy();
	}
}


void ASTrackerBot::MulticastExplode_Implementation()
{
	if (ExplosionEffect)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());

	if (ExplodeSound)
		UGameplayStatics::PlaySoundAtLocation(this, ExplodeSound, GetActorLocation());
}


void ASTrackerBot::OnDamaged(int32 Damage)
{
	MaterialPulseLoopForward();
}


// Called every frame
void ASTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Role == ROLE_Authority)
	{
		float DistanceToTarget = (GetActorLocation() - NextPathPoint).Size();

		if (DistanceToTarget <= RequiredDistanceToTarget)
		{
			NextPathPoint = GetNextPathPoint();
		}
		else
		{
			FVector ForceDirection = NextPathPoint - GetActorLocation();
			ForceDirection.Normalize();
			FVector TwoVer = GetVelocity();
			TwoVer.Normalize();
			if(ForceDirection != TwoVer)
				MeshComp->AddForce(GetVelocity() * -2, NAME_None, bUseVelocityChange);

			ForceDirection *= MovementForce;

			MeshComp->AddForce(ForceDirection, NAME_None, bUseVelocityChange);
		}


		if (InterestingLocation.bIsSet && (GetActorLocation() - InterestingLocation.Location).Size() < 80.f )
		{
			InterestingLocation.Checked();
			if (SecInterestingLocation.bIsSet)
			{
				InterestingLocation.SetLocation(SecInterestingLocation.Location);
				SecInterestingLocation.Checked();
			}
		}


		if (NextPatrolPoint && (GetActorLocation() - NextPatrolPoint->GetActorLocation()).Size() < 120.f)
		{
			int8 Index = PatrolPoints.Find(NextPatrolPoint) + 1;
			if (Index < PatrolPoints.Num())
				NextPatrolPoint = PatrolPoints[Index];
			else
				NextPatrolPoint = PatrolPoints[0];
		}
	}
	
}


void ASTrackerBot::MaterialPulseLoopForward()
{
	if (MatInst == nullptr)
	{
		MatInst = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComp->GetMaterial(0));
	}

	if (MatInst)
	{
		MaterialColor += FLinearColor(0.15f, -0.18f, 0.13f) * GetWorld()->DeltaTimeSeconds;
		if (MaterialColor.R >= 0.2f)
		{
			MaterialColor = FLinearColor(0.2f, 0.02f, 0.2f);
			GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ASTrackerBot::MaterialPulseLoopBackward);
		}
		else
		{	
			GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ASTrackerBot::MaterialPulseLoopForward);
		}
		MatInst->SetVectorParameterValue("DiffuseColor", MaterialColor);
	}
}


void ASTrackerBot::MaterialPulseLoopBackward()
{
	if (MatInst == nullptr)
	{
		MatInst = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComp->GetMaterial(0));
	}

	if (MatInst)
	{
		MaterialColor -= FLinearColor(0.15f, -0.18f, 0.13f) * GetWorld()->DeltaTimeSeconds;
		if (MaterialColor.R <= 0.05f)
		{
			MaterialColor = FLinearColor(0.05f, 0.2f, 0.07f);
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ASTrackerBot::MaterialPulseLoopBackward);
		}
		MatInst->SetVectorParameterValue("DiffuseColor", MaterialColor);
	}
}


void ASTrackerBot::RefreshPath()
{
	NextPathPoint = GetNextPathPoint();
}


void ASTrackerBot::OnCDSBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Role != ROLE_Authority)
		return;
	if (TargetsInRange.Contains(OtherActor))
		Explode();
}


void ASTrackerBot::OnADSBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Role != ROLE_Authority)
		return;
	if (OtherActor->GetClass()->ImplementsInterface(UGameplayTagAssetInterface::StaticClass()))
	{
		FGameplayTagContainer OtherActorTags;
		Cast<IGameplayTagAssetInterface>(OtherActor)->GetOwnedGameplayTags(OtherActorTags);
		if (OtherActorTags.HasTag(UGameplayTagsManager::Get().RequestGameplayTag("Fraction.Player")))
		{
			TargetsInRange.Add(OtherActor);
		}
	}
}


void ASTrackerBot::OnADSEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Role != ROLE_Authority)
		return;
	if (TargetsInRange.Contains(OtherActor))
		TargetsInRange.Remove(OtherActor);
}


void ASTrackerBot::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Role != ROLE_Authority)
		return;
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	FVector HitPower = HitNormal * GetVelocity();
	if (HitPower.X + HitPower.Y + HitPower.Z > 150)
		Explode();
}


bool ASTrackerBot::IsTargetInView(AActor* Actor)
{
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.bTraceComplex = true;
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = Actor->GetActorLocation();
	GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECollisionChannel::ECC_Camera, Params);

	return Hit.GetActor() == Actor;
}


void ASTrackerBot::InterestingLocationDetected(FVector Location)
{
	if (Role != ROLE_Authority)
		return;
	if (Location.Z > GetActorLocation().Z + 100 || Location.Z < GetActorLocation().Z - 100)
		return;
	if (!InterestingLocation.bIsSet || (Location - InterestingLocation.Location).Size() < 200.f)
	{
		InterestingLocation.SetLocation(Location);
	}
	else
	{
		SecInterestingLocation.SetLocation(Location);
	}
}