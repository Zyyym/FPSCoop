// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "FPSCoopCharacter.h"
#include "FPSCoopProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Components/HealthComponent.h"
#include "Projectiles/AmmoType.h"
#include "Player/FPSPlayerCon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/ProjectileWeapon.h"
#include "GameplayTags.h"
#include "Game/MultiplayerGM.h"
#include "Interactive/MissionObjective.h"


DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);


AFPSCoopCharacter::AFPSCoopCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f);
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	GetMesh()->bOwnerNoSee = true;

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));;
	HealthComp->OnKilled.AddDynamic(this, &AFPSCoopCharacter::OnKilled);

	GameplayTags.AddTag(UGameplayTagsManager::Get().RequestGameplayTag("Fraction.Player"));

	bHaveObjective = false;
}


void AFPSCoopCharacter::BeginPlay()
{ 
	Super::BeginPlay();
	if (Role == ROLE_Authority)
	{
		FVector Location(0.0f, 0.0f, 0.0f);
		FRotator Rotation(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnInfo.Owner = this;
		AWeapon* Temp;
		Weapons.Empty();
		for (TSubclassOf<AWeapon>& Cla : WeaponsClasses)
		{
			Temp = GetWorld()->SpawnActor<AWeapon>(Cla, Location, Rotation, SpawnInfo);
			Weapons.Add(Temp);
			Temp->SetMyPawn(this);
			Temp->SetActorHiddenInGame(true);
			Temp->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
		}

		NextWeapon();
		if (IsLocallyControlled())
			GetController<AFPSPlayerCon>()->CreateWeaponBar(Weapons);
	}
	
}


void AFPSCoopCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Role == ROLE_Authority)
	{
		NewRot = FirstPersonCameraComponent->RelativeRotation;
		NewRot.Pitch = GetControlRotation().Pitch;

		FirstPersonCameraComponent->SetRelativeRotation(NewRot);
	}
}


void AFPSCoopCharacter::OnRep_SetRot()
{
	FirstPersonCameraComponent->SetRelativeRotation(NewRot);
}


void AFPSCoopCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, this, &AFPSCoopCharacter::NextWeapon);
	PlayerInputComponent->BindAction("PreviousWeapon", IE_Pressed, this, &AFPSCoopCharacter::PreviousWeapon);

	PlayerInputComponent->BindAction("NextAmmo", IE_Pressed, this, &AFPSCoopCharacter::NextAmmo);
	PlayerInputComponent->BindAction("PreviousAmmo", IE_Pressed, this, &AFPSCoopCharacter::PreviousAmmo);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCoopCharacter::OnFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AFPSCoopCharacter::OnStopFire);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCoopCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCoopCharacter::MoveRight);

	//PlayerInputComponent->BindAction("OpenInGameMenu", IE_Pressed, this, &AFPSCoopCharacter::OpenInGameMenu);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFPSCoopCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFPSCoopCharacter::LookUpAtRate);
}


void AFPSCoopCharacter::AddProjectileAmmo(TSubclassOf<UAmmoType> AmmoClass, int32 Amount)
{
	AProjectileWeapon* ProjectileWeapon;
	for (AWeapon* We : Weapons)
	{
		ProjectileWeapon = Cast<AProjectileWeapon>(We);
		if (ProjectileWeapon)
		{
			ProjectileWeapon->AddAmmo(AmmoClass, Amount);
			break;
		}
	}	
}


void AFPSCoopCharacter::OnFire()
{
	if (Weapon)
		Weapon->StartShooting();
}


void AFPSCoopCharacter::OnStopFire()
{
	if (Weapon)
		Weapon->EndShooting();
}


void AFPSCoopCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFPSCoopCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AFPSCoopCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFPSCoopCharacter::LookUpAtRate(float Rate)
{	
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}


void AFPSCoopCharacter::NextWeapon()
{
	ServerSwitchWeapon(true);
}


void AFPSCoopCharacter::PreviousWeapon()
{
	ServerSwitchWeapon(false);
}


void AFPSCoopCharacter::ServerSwitchWeapon_Implementation(bool bNext)
{
	if (!Weapon)
	{
		if (Weapons.Num() > 0)
		{
			Weapon = Weapons[0];
			Weapon->SetActorHiddenInGame(false);
		}
		return;
	}

	if (Weapons.Num() == 1)
		return;

	Weapon->WeaponSwapedSecondary();

	if (bNext)
	{
		if (Weapons.Find(Weapon) < Weapons.Num() - 1)
		{
			Weapon = Weapons[Weapons.Find(Weapon) + 1];
		}
		else
			Weapon = Weapons[0];
	}
	else
	{
		if (Weapons.Find(Weapon) > 0)
		{
			Weapon = Weapons[Weapons.Find(Weapon) - 1];
		}
		else
			Weapon = Weapons.Last();
	}
	Weapon->WeaponSwapedPrimary();
}


bool AFPSCoopCharacter::ServerSwitchWeapon_Validate(bool bNext)
{
	return true;
}



void AFPSCoopCharacter::NextAmmo()
{
	AProjectileWeapon* PWeap = Cast<AProjectileWeapon>(Weapon);
	if (PWeap)
	{
		PWeap->NextAmmo();
	}
}

void AFPSCoopCharacter::PreviousAmmo()
{
	AProjectileWeapon* PWeap = Cast<AProjectileWeapon>(Weapon);
	if (PWeap)
	{
		PWeap->PreviousAmmo();
	}
}


void AFPSCoopCharacter::OpenInGameMenu()
{
	AFPSPlayerCon* MyPC = GetController<AFPSPlayerCon>();
	if (MyPC)
		MyPC->InGameMenuOpened();
}


void AFPSCoopCharacter::TakeObjective()
{
	bHaveObjective = true;
	AMultiplayerGM* MGM = Cast<AMultiplayerGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (MGM)
	{
		MGM->UpdateMisionStatus();
	}
}


void AFPSCoopCharacter::OnKilled()
{
	if (bHaveObjective)
	{
		bHaveObjective = false;
		AMultiplayerGM* MGM = Cast<AMultiplayerGM>(UGameplayStatics::GetGameMode(GetWorld()));
		if (MGM)
		{
			MGM->PawnLostObjective();
		} 

		if (Role == ROLE_Authority && ObjectiveClass)
		{
			const FRotator SpawnRotation = GetActorRotation();
			const FVector SpawnLocation = GetActorLocation() + FVector(0, 0, -70);

			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			ActorSpawnParams.Owner = this;

			GetWorld()->SpawnActor<AMissionObjective>(ObjectiveClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}

	if (Role == ROLE_Authority)
	{	
		GetController<AFPSPlayerCon>()->Blurout();
		Destroy();
	}
}


void AFPSCoopCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSCoopCharacter, Weapons);
	DOREPLIFETIME(AFPSCoopCharacter, WeaponsClasses);
	DOREPLIFETIME(AFPSCoopCharacter, Weapon);
	DOREPLIFETIME(AFPSCoopCharacter, NewRot);
}