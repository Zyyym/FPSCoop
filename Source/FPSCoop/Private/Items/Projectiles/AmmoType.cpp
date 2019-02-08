// Fill out your copyright notice in the Description page of Project Settings.

#include "AmmoType.h"
#include "Engine/World.h"
#include "Public/TimerManager.h"
#include "Net/UnrealNetwork.h"
#include "UI/AmmoWidget.h"

UAmmoType::UAmmoType(const FObjectInitializer & X):Super(X)
{
	bCanShoot = true;
	IsPrimary = -1;
}

int32 UAmmoType::AddAmmo(int32 Amount)
{
	int32 AmmoLeft = FMath::Clamp(Amount + Ammo - MaxAmmo, 0, Amount);
	Ammo = FMath::Clamp(Ammo + Amount, 0, MaxAmmo);
	if (MyWidget)
		MyWidget->Refresh();
	return AmmoLeft;
}


bool UAmmoType::Shot()
{
	if (Ammo <= 0)
		return false;

	Ammo--;
	if (MyWidget)
		MyWidget->Refresh(); //Aktualizuje widget gdy jest serwerem 
	return true;
}


void UAmmoType::OnRep_SetAmmo()
{
	if (MyWidget)
		MyWidget->Refresh(); //Aktualizuje widget gdy jest clientem
}


void UAmmoType::OnRep_SetIsPrimary()
{
	if (MyWidget)
	{
		if (IsPrimary == 1)
		{
			MyWidget->MakePrimary();
		}
		else
		{
			MyWidget->MakeSecondary();
		}
	}
		
}


void UAmmoType::SetWidget(UAmmoWidget* NewWidget)
{
	MyWidget = NewWidget;
}


bool UAmmoType::CanShoot() const
{ 
	return bCanShoot; 
}


FSlateBrush UAmmoType::GetImage() const
{ 
	return Image; 
}


int32 UAmmoType::GetAmmo() const
{ 
	return Ammo; 
}


TSubclassOf<AFPSCoopProjectile> UAmmoType::GetAmmoType() const
{
	return AmmoType;
}


float UAmmoType::GetRegenerationTime() const
{
	if (Ammo == 0)
		return 0;

	return RegenerationTime;
}


void UAmmoType::MakePrimary()
{
	IsPrimary = 1;
	if (MyWidget)
	{
		MyWidget->MakePrimary();
	}
}


void UAmmoType::MakeSecondary()
{
	IsPrimary = 0;
	if (MyWidget)
	{
		MyWidget->MakeSecondary();
	}
}


void UAmmoType::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAmmoType, Ammo);
	DOREPLIFETIME(UAmmoType, IsPrimary);
	DOREPLIFETIME(UAmmoType, bReplicatedFlag);
}

