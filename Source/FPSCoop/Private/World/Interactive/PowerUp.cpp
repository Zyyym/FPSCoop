// Fill out your copyright notice in the Description page of Project Settings.

#include "PowerUp.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "FPSCoopCharacter.h"
#include "Net/UnrealNetwork.h"

APowerUp::APowerUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RelativeLocation = FVector(0, 0, 70);
	MeshComp->SetRelativeLocation(RelativeLocation);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	BoxCollision->SetupAttachment(RootComponent);
	BoxCollision->SetBoxExtent(FVector(50, 50, 70));
	BoxCollision->SetRelativeLocation(FVector(0, 0, 35));

	EffectScale = FVector(0.5f, 0.5f, 0.5f);
	SetReplicates(true);
}

// Called when the game starts or when spawned
void APowerUp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Role == ROLE_Authority)
	{
		if (bFlyUp)
		{
			if (RelativeLocation.Z > 70)
				RelativeLocation += FVector(0, 0, (FMath::Clamp((100 - RelativeLocation.Z) * 4, 7.5f, 75.f) * DeltaTime));
			else
				RelativeLocation += FVector(0, 0, (FMath::Clamp((RelativeLocation.Z - 40) * 4, 7.5f, 75.f) * DeltaTime));
			MeshComp->SetRelativeLocation(RelativeLocation);
			if (RelativeLocation.Z > 100)
			{
				RelativeLocation = FVector(0, 0, 100);
				bFlyUp = false;
			}
		}
		else
		{
			if (RelativeLocation.Z > 70)
				RelativeLocation -= FVector(0, 0, (FMath::Clamp((100 - RelativeLocation.Z) * 4, 7.5f, 75.f) * DeltaTime));
			else
				RelativeLocation -= FVector(0, 0, (FMath::Clamp((RelativeLocation.Z - 40) * 4, 7.5f, 75.f) * DeltaTime));

			MeshComp->SetRelativeLocation(RelativeLocation);
			if (RelativeLocation.Z < 40)
			{
				RelativeLocation = FVector(0, 0, 40);
				bFlyUp = true;
			}
		}
	}
}


void APowerUp::OnRep_SetLocation()
{
	MeshComp->SetRelativeLocation(RelativeLocation);
}


void APowerUp::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	AFPSCoopCharacter* Hero = Cast<AFPSCoopCharacter>(OtherActor);
	if (!Hero)
		return;
	if (TakeEffect)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TakeEffect, GetActorLocation(), FRotator(0.f, 0.f, 0.f), EffectScale);
	if (TakeSound)
		UGameplayStatics::PlaySoundAtLocation(this, TakeSound, GetActorLocation(), .1f);

	if (Role == ROLE_Authority)
		Destroy();

}


void APowerUp::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APowerUp, RelativeLocation);
}