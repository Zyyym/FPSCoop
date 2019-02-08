// Fill out your copyright notice in the Description page of Project Settings.

#include "WayOut.h"
#include "FPSCoopCharacter.h"
#include "Game/MultiplayerGM.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Particles/ParticleSystem.h"



void AWayOut::BeginPlay()
{
	if(Particle)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, GetActorLocation(), FRotator(0.f, 0.f, 0.f), EffectScale);
}


void AWayOut::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (Role == ROLE_Authority)
	{
		AFPSCoopCharacter* Hero = Cast<AFPSCoopCharacter>(OtherActor);
		if (Hero && Hero->HaveObjective())
		{
			AMultiplayerGM* MGM = Cast<AMultiplayerGM>(UGameplayStatics::GetGameMode(GetWorld()));
			if (MGM)
			{
				MGM->GameOver(true);
			}
		}
	}
}