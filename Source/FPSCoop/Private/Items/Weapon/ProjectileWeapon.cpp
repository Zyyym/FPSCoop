// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileWeapon.h"
#include "Engine/World.h"
#include "FPSCoopProjectile.h"
#include "Projectiles/AmmoType.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "UI/AmmoBar.h"
#include "Engine/ActorChannel.h"

void AProjectileWeapon::BeginPlay()
{
	/*if (Role == ROLE_Authority)
	{	
		Ammos.Empty();
		for (TSubclassOf<UAmmoType>& Cla : AmmoClasses)
		{
			Ammos.Add(NewObject<UAmmoType>(this, Cla));
		}

		Projectile = Ammos[0];
	}*/
}


void AProjectileWeapon::Shot()
{
	if (Projectile->GetAmmo() == 0)
		return;
	ServerShot();

	Super::Shot();
}


void AProjectileWeapon::ServerShot_Implementation()
{
	if (Projectile != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			if (!Projectile->CanShoot())
				return;
			if (!Projectile->Shot())
			{
				//
				return;
			}

			const FRotator SpawnRotation = MyPawn->GetControlRotation();
			const FVector SpawnLocation = FP_MuzzleLocation->GetComponentLocation();

			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			ActorSpawnParams.Owner = this;

			World->SpawnActor<AFPSCoopProjectile>(Projectile->GetAmmoType(), SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}
}


bool AProjectileWeapon::ServerShot_Validate()
{
	return true;
}


void AProjectileWeapon::NextAmmo()
{
	ServerNextAmmo();
}


void AProjectileWeapon::ServerNextAmmo_Implementation()
{
	Projectile->MakeSecondary();

	if (Ammos.Find(Projectile) < Ammos.Num() - 1)
	{
		Projectile = Ammos[Ammos.Find(Projectile) + 1];
	}
	else
		Projectile = Ammos[0];

	Projectile->MakePrimary();
}

bool AProjectileWeapon::ServerNextAmmo_Validate()
{
	return true;
}


void AProjectileWeapon::PreviousAmmo()
{
	ServerPreviousAmmo();
}


void AProjectileWeapon::ServerPreviousAmmo_Implementation()
{
	Projectile->MakeSecondary();

	if (Ammos.Find(Projectile) > 0)
	{
		Projectile = Ammos[Ammos.Find(Projectile) - 1];
	}
	else
		Projectile = Ammos.Last();

	Projectile->MakePrimary();
}

bool AProjectileWeapon::ServerPreviousAmmo_Validate()
{
	return true;
}


float AProjectileWeapon::GetRegenerationTime() const
{
	return Projectile->GetRegenerationTime();
}


UAmmoBar* AProjectileWeapon::GetAmmoBar()
{
	UAmmoBar* Bar = CreateWidget<UAmmoBar>(GetWorld(), AmmoBarClass);
	Bar->CreateBar(Ammos);
	return Bar;
}


void AProjectileWeapon::AddAmmo(TSubclassOf<UAmmoType> AmmoClass, int32 Amount)
{
	for (UAmmoType* Am : Ammos)
	{
		if (Am->GetClass() == AmmoClass)
		{
			Am->AddAmmo(Amount);
			break;
		}
	}
}


void AProjectileWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HasAuthority())
	{
		for (TSubclassOf<UAmmoType>& Cla : AmmoClasses)
		{
			Ammos.Add(NewObject<UAmmoType>(this, Cla));
		}

		Projectile = Ammos[0];
	}
}


bool AProjectileWeapon::ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags)
{
	bool bRep = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	if (Projectile != nullptr)
	{
		bRep |= Channel->ReplicateSubobject(Projectile, *Bunch, *RepFlags);
	}

	for (UAmmoType* Am : Ammos)
	{
		if(Am)
			bRep |= Channel->ReplicateSubobject(Am, *Bunch, *RepFlags);
	}

	return bRep;
}


void AProjectileWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AProjectileWeapon, Projectile);
	DOREPLIFETIME(AProjectileWeapon, Ammos);
	DOREPLIFETIME(AProjectileWeapon, AmmoClasses);
}
