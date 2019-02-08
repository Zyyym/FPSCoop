// Fill out your copyright notice in the Description page of Project Settings.

#include "TrackerBot.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/HealthComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "NavigationSystem/Public/NavigationPath.h"
#include "Library/ActionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Public/TimerManager.h"
#include "Components/SphereComponent.h"
#include "GameplayTags.h"
#include "Engine/TargetPoint.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ATrackerBot::ATrackerBot()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsPathSet = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetNotifyRigidBodyCollision(true);
	MeshComp->SetCanEverAffectNavigation(false);

	CloseDetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CloseDetectionSphere"));
	CloseDetectionSphere->SetupAttachment(RootComponent);
	CloseDetectionSphere->SetSphereRadius(70.f);
	CloseDetectionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	CloseDetectionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CloseDetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &ATrackerBot::OnCDSBeginOverlap);

	ActorDetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ActorDetectionSphere"));
	ActorDetectionSphere->SetupAttachment(RootComponent);
	ActorDetectionSphere->SetSphereRadius(500.f);
	ActorDetectionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	ActorDetectionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	ActorDetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &ATrackerBot::OnADSBeginOverlap);
	ActorDetectionSphere->OnComponentEndOverlap.AddDynamic(this, &ATrackerBot::OnADSEndOverlap);

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->OnHearNoise.AddDynamic(this, &ATrackerBot::OnNoiseHeard);
	PawnSensing->HearingThreshold = 3000.f;
	PawnSensing->LOSHearingThreshold = 1800.f;

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));

	AIState = EAIMode::AIM_Stand;
}

// Called when the game starts or when spawned
void ATrackerBot::BeginPlay()
{
	Super::BeginPlay();

	HomeLocation = GetActorLocation();

	if (Role = ROLE_Authority)
	{
		if (!NextPatrolPoint && PatrolPoints.Num() > 0)
		{
			SetNextPatrolPoint();		
		}
		if(NextPatrolPoint)
			AIState = EAIMode::AIM_Patrol;
		RefreshPath();
	}
	
}

// Called every frame
void ATrackerBot::Tick(float DeltaTime)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, HomeLocation.ToString());
	Super::Tick(DeltaTime);
	if (Role != ROLE_Authority)
		return;
	if (bIsPathSet && Role == ROLE_Authority)
	{
		FollowTarget();	}
	
}

// Called to bind functionality to input
void ATrackerBot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void ATrackerBot::FollowTarget()
{
	if (Role != ROLE_Authority)
	{
		return;
	}
	FVector ForceDirection = NextPathPoint - GetActorLocation();

	if (ForceDirection.Size() < 50.f)
	{ 
		if (AIState == EAIMode::AIM_Stand)
			return;

		if (AIState == EAIMode::AIM_Patrol)
			SetNextPatrolPoint();

		if (AIState == EAIMode::AIM_Looking)
		{
			if (NextTargetLocation.IsSet)
			{
				TargetLocation.SetLocation(NextTargetLocation.Location);
				NextTargetLocation.Checked();
			}
			else
				TargetLocation.Checked();
		}

		RefreshPath();
	}

	ForceDirection.Normalize();
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, ForceDirection.ToString());
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, NextPathPoint.ToString());

	ForceDirection *= 500;

	MeshComp->AddForce(ForceDirection, NAME_None, true);
}



void ATrackerBot::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{	
	if (Role != ROLE_Authority)
		return;

	if (Target)
		return;

	if (!TargetLocation.IsSet || (TargetLocation.Location - Location).Size() < 200.f * Volume)
	{
		TargetLocation.SetLocation(Location);
		RefreshPath();
	}
	else
	{
		NextTargetLocation.SetLocation(Location);
	}
	
}


void ATrackerBot::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	FVector HitPowerVector = HitNormal * GetVelocity();
	//Sprawdzic takze X > "WymaganaSi³a" || Y > "WymaganaSi³a" || Z > "WymaganaSi³a"
	float HitPower = HitPowerVector.X + HitPowerVector.Y + HitPowerVector.Y;

	/*UE_LOG(LogTemp, Log, TEXT(FString::SanitizeFloat(HitPower)));
	UE_LOG(LogTemp, Log, TEXT(HitPowerVector.ToString()));
	UE_LOG(LogTemp, Log, TEXT(HitNormal.ToString()));
	UE_LOG(LogTemp, Log, TEXT(NormalImpulse.ToString()));*/

}


FVector ATrackerBot::GetNextPathPoint()
{
	UNavigationPath* NavPath;
	//Przed switchem ze wzgledu na b³¹d kompilacji "U¿ycie potencjalnie niezainicjowanej, lokalnej zmiennej  wskaznikowej "NavPath""
	NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), GetActorLocation(), HomeLocation);
	switch (AIState)
	{
	case AIM_Stand:
		break;
	case AIM_Patrol: 
	{
		if (NextPatrolPoint)
			NavPath = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), GetActorLocation(), NextPatrolPoint);
		else if (PatrolPoints.Num() > 0)
		{
			SetNextPatrolPoint();
			return GetNextPathPoint();
		}
		else
		{
			AIState = EAIMode::AIM_Stand;
			return GetNextPathPoint();
		}
	}
		break;
	case AIM_Looking:
	{
		if (TargetLocation.IsSet)
			NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), GetActorLocation(), TargetLocation.Location);
		else
		{
			AIState = EAIMode::AIM_Patrol;
			return GetNextPathPoint();
		}
	}
		break;
	case AIM_Agro:
	{
		if (Target)
			NavPath = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), GetActorLocation(), Target);	
		else
		{
			AIState = EAIMode::AIM_Looking;
			return GetNextPathPoint();
		}
	}
		break;
	default:
		break;
	}

	if (NavPath && NavPath->PathPoints.Num() > 1)
	{
		return NavPath->PathPoints[1];
	}
	return FVector();
}


void ATrackerBot::Explode()
{
	TArray<AActor*> Ignore;
	Ignore.Add(this);

	UActionLibrary::AddRadialImpulseToComponents(GetActorLocation(), 400.f, 600.f, Ignore);

	UActionLibrary::AddRadialImpulseToPawns(GetActorLocation(), 400.f, 600.f, Ignore, 2);

	Destroy();
}


void ATrackerBot::RefreshPath()
{
	bIsPathSet = true;
	NextPathPoint = GetNextPathPoint();
	GetWorldTimerManager().ClearTimer(RefreshPathTimer);
	GetWorld()->GetTimerManager().SetTimer(RefreshPathTimer, this, &ATrackerBot::RefreshPath, 3.f);
}


void ATrackerBot::SetNextPatrolPoint()
{
	if (PatrolPoints.Num() == 0)
	{
		AIState = EAIMode::AIM_Stand;
		return;
	}

	if (!NextPatrolPoint)
	{
		NextPatrolPoint = PatrolPoints[0];
		return;
	}

	int8 Index = PatrolPoints.Find(NextPatrolPoint) + 1;
	if (Index < PatrolPoints.Num())
		NextPatrolPoint = PatrolPoints[Index];
	else
		NextPatrolPoint = PatrolPoints[0];
}


void ATrackerBot::OnCDSBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Role != ROLE_Authority)
		return;

	if (OtherActor->GetClass()->ImplementsInterface(UGameplayTagAssetInterface::StaticClass()))
	{
		FGameplayTagContainer OtherActorTags;
		Cast<IGameplayTagAssetInterface>(GetOwner())->GetOwnedGameplayTags(OtherActorTags);
		if (OtherActorTags.HasTag(UGameplayTagsManager::Get().RequestGameplayTag("Fraction.Player")))
		{
			Explode();
		}
	}
}


void ATrackerBot::OnADSBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Role != ROLE_Authority)
		return;

	if (OtherActor->GetClass()->ImplementsInterface(UGameplayTagAssetInterface::StaticClass()))
	{
		FGameplayTagContainer OtherActorTags;
		Cast<IGameplayTagAssetInterface>(GetOwner())->GetOwnedGameplayTags(OtherActorTags);
		if (OtherActorTags.HasTag(UGameplayTagsManager::Get().RequestGameplayTag("Fraction.Player")))
		{
			TargetsInRange.Add(OtherActor);
			if (!Target)
			{
				Target = OtherActor;
				TargetLocation.Checked();
			}
			RefreshPath();
		}
	}
}


void ATrackerBot::OnADSEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Role != ROLE_Authority)
		return;

	if (TargetsInRange.Contains(OtherActor))
	{
		TargetsInRange.Remove(OtherActor);
		if (OtherActor == Target)
		{
			TargetLocation.SetLocation(Target->GetActorLocation());
			Target = nullptr;
			RefreshPath();
		}
	}
}


void ATrackerBot::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATrackerBot, HomeLocation);
	DOREPLIFETIME(ATrackerBot, NextPathPoint);
	DOREPLIFETIME(ATrackerBot, AIState);
	//DOREPLIFETIME(ATrackerBot, Weapon);
}
