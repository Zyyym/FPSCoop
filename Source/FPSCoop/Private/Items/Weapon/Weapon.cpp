// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "UI/WeaponWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "Player/FPSPlayerCon.h"
#include "TimerManager.h"

AWeapon::AWeapon()
{
	ShotMode = EShotType::ST_Single;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	SetActorEnableCollision(false);

	SetReplicates(true);
	SetReplicateMovement(true);
	MinNetUpdateFrequency = 33.3f;
	NetUpdateFrequency = 66.6f;
	bCanShot = true;
}


void AWeapon::BeginPlay()
{

}


void AWeapon::SetMyPawn(APawn* MyNewPawn)
{ 
	MyPawn = MyNewPawn; 
}


void AWeapon::WeaponSwapedPrimary()
{
	ClientUpdateWidget(true);
	SetActorHiddenInGame(false);
}


void AWeapon::WeaponSwapedSecondary()
{
	ClientUpdateWidget(false);
	SetActorHiddenInGame(true);
}


void AWeapon::ClientUpdateWidget_Implementation(bool bPrimary)
{
	if (Widget)
	{
		if(bPrimary)
			Widget->MakePrimary();
		else
			Widget->MakeSecondary();
	}			
}


void AWeapon::StartShooting()
{
	ServerStartShoting();
}


void AWeapon::ServerStartShoting_Implementation()
{
	switch (ShotMode)
	{
	case EShotType::ST_Single:
	{
		if (bCanShot)
		{
			ClientShowRegeneration(GetRegenerationTime());
			Shot();
		}
	}
	break;
	case EShotType::ST_Burst:
	{

	}
	break;
	case EShotType::ST_Auto:
	{

	}
	break;
	case EShotType::ST_Hold:
	{
		Shot();
	}
	break;
	default:
		break;
	}
}


bool AWeapon::ServerStartShoting_Validate()
{
	return true;
}


void AWeapon::ClientShowRegeneration_Implementation(float Time)
{
	if (Time <= 0)
		return;
	if (Widget)
		Widget->StartMakeUsable(Time);
}


void AWeapon::EndShooting()
{

}


void AWeapon::Shot()
{
	MulticastPlayShotSound();

	if (Role == ROLE_Authority)
	{
		const float RegTime = GetRegenerationTime();
		if (RegTime > 0)
		{
			bCanShot = false;
			FTimerHandle RegenTimer;
			GetWorld()->GetTimerManager().SetTimer(RegenTimer, this, &AWeapon::Regenerate, RegTime);
		}
	}

}


void AWeapon::MulticastPlayShotSound_Implementation()
{
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, FP_MuzzleLocation->GetComponentLocation(), .1f);
	}
}


void AWeapon::SetWidget(class UWeaponWidget* NewWidget)
{
	Widget = NewWidget;
}


float AWeapon::GetRegenerationTime() const
{
	return 0;
}

int32 AWeapon::GetAmmo() const
{
	return -2;
}


void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, ShotMode);
	DOREPLIFETIME(AWeapon, MyPawn);
}