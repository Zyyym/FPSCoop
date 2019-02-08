// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponGraviter.h"
#include "GameFramework/Pawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AWeaponGraviter::AWeaponGraviter()
{
	Ammo = -1;
	Range = 500.f;
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}


void AWeaponGraviter::StartShooting()
{	
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation(), .1f);
	}
	SeverStartShooting();
}


void AWeaponGraviter::SeverStartShooting_Implementation()
{
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(MyPawn);
	Params.bTraceComplex = true;
	FVector StartLocation = FP_MuzzleLocation->GetComponentLocation();
	FVector EndLocation = StartLocation + UKismetMathLibrary::GetForwardVector(MyPawn->GetControlRotation()) * Range;
	GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECollisionChannel::ECC_PhysicsBody, Params);

	if (Hit.GetComponent())
	{
		HoldedPrimitive = Hit.GetComponent();
		HoldedPrimitive->SetEnableGravity(false);
	}
}


bool AWeaponGraviter::SeverStartShooting_Validate()
{
	return true;
}


void AWeaponGraviter::EndShooting()
{
	SeverEndShooting();
}


void AWeaponGraviter::SeverEndShooting_Implementation()
{
	if (HoldedPrimitive)
		HoldedPrimitive->SetEnableGravity(true);
	HoldedPrimitive = nullptr;
}


bool AWeaponGraviter::SeverEndShooting_Validate()
{
	return true;
}