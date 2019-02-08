// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "UI/HealthBar.h"
#include "Player/FPSPlayerCon.h"
#include "TimerManager.h"
#include "FPSCoopCharacter.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	DefHealth = 4;
	MaxHealth = DefHealth;
	Health = MaxHealth;
	bShowDamage = true;

	SetIsReplicated(true);
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

}


void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


void UHealthComponent::DealDamage(int32 Damage)
{
	if (Damage < 0)
	{
		ApplyHeal(-Damage);
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0, MaxHealth);

	if (bShowDamage)
	{
		AFPSPlayerCon* Cont;
		for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
		{
			Cont = Cast<AFPSPlayerCon>(It->Get());
			if (Cont && Cont->GetPawn() && Cont->GetPawn() != GetOwner())
			{
				FHitResult Hit;
				FCollisionQueryParams Params;
				Params.AddIgnoredActor(GetOwner());
				Params.bTraceComplex = true;
				FVector StartLocation = GetOwner()->GetActorLocation();
				FVector EndLocation = Cast<AFPSCoopCharacter>(Cont->GetPawn())->GetFirstPersonCameraComponent()->GetComponentLocation();
				GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECollisionChannel::ECC_Camera, Params);

				if (Hit.GetActor() == Cont->GetPawn())
					Cont->ShowDamageText(Damage, GetOwner()->GetActorLocation());
			}
		}
	}

	OnDamageTaken.Broadcast(Damage);

	if (HealthBar)
		HealthBar->RefreshBar();

	if (Health == 0)
		Kill();
}


void UHealthComponent::ApplyHeal(int32 Heal)
{
	if (Heal < 0)
	{
		ApplyHeal(-Heal);
		return;
	}

	Health = FMath::Clamp(Health - Heal, 0, MaxHealth);

	if (bShowDamage)
	{
		AFPSPlayerCon* Cont;
		for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
		{
			Cont = Cast<AFPSPlayerCon>(It->Get());
			if (Cont && Cont->GetPawn() != GetOwner())
			{
				FHitResult Hit;
				FCollisionQueryParams Params;
				Params.AddIgnoredActor(GetOwner());
				Params.bTraceComplex = true;
				FVector StartLocation = GetOwner()->GetActorLocation();
				FVector EndLocation = Cast<AFPSCoopCharacter>(Cont->GetPawn())->GetFirstPersonCameraComponent()->GetComponentLocation();
				GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECollisionChannel::ECC_Camera, Params);

				if(Hit.GetActor() == Cont->GetPawn())
					Cont->ShowDamageText(Heal, GetOwner()->GetActorLocation(), true);			
			}
		}
	}

	OnHealed.Broadcast(Heal);

	if (HealthBar)
		HealthBar->RefreshBar();

	if (Health == 0)
		Kill();
}


void UHealthComponent::OnRep_SetHealth()
{
	if (HealthBar)
		HealthBar->RefreshBar();
}


void UHealthComponent::Kill()
{
	OnKilled.Broadcast();
}


void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, Health);
}
