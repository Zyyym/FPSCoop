// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponData.h"
#include "Engine/World.h"
#include "Public/TimerManager.h"
#include "Net/UnrealNetwork.h"

UWeaponData::UWeaponData()
{
	bCanShoot = true;
}

int32 UWeaponData::AddAmmo(int32 AddedAmmo)
{
	int32 AmmoLeft = FMath::Clamp(AddedAmmo - MaxAmmo, 0, AddedAmmo);
	Ammo = FMath::Clamp(Ammo + AddedAmmo, 0, MaxAmmo);
	return AmmoLeft;
}


bool UWeaponData::Shot()
{
	/*FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &UWeaponData::Regenerate, RegenerationTime, false);*/

	if (Ammo <= 0)
		return false;

	Ammo--;
	return true;
}


void UWeaponData::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UWeaponData, Ammo);
}