// Fill out your copyright notice in the Description page of Project Settings.

#include "MissionObjective.h"
#include "FPSCoopCharacter.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"

void AMissionObjective::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (Role == ROLE_Authority && bCanBeTaken)
	{
		AFPSCoopCharacter* Hero = Cast<AFPSCoopCharacter>(OtherActor);
		if (Hero)
		{
			Hero->TakeObjective();
		}
	}
	if(bCanBeTaken)
		Super::NotifyActorBeginOverlap(OtherActor);
}


void AMissionObjective::BeginPlay()
{
	Super::BeginPlay();
	bCanBeTaken = false;
	FTimerHandle AvaibleTimer;
	GetWorld()->GetTimerManager().SetTimer(AvaibleTimer, this, &AMissionObjective::StartBeAvaible, 1);
}


void AMissionObjective::StartBeAvaible()
{
	bCanBeTaken = true;
	UpdateOverlaps();
}


void AMissionObjective::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMissionObjective, bCanBeTaken);
}