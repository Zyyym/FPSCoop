// Fill out your copyright notice in the Description page of Project Settings.

#include "AmmoGiver.h"
#include "GameplayTags.h"
#include "FPSCoopCharacter.h"

void AAmmoGiver::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (Role == ROLE_Authority)
	{
		AFPSCoopCharacter* Hero = Cast<AFPSCoopCharacter>(OtherActor);
		if (Hero)
		{
			Hero->AddProjectileAmmo(AmmoType, Amount);
		}
	}
	Super::NotifyActorBeginOverlap(OtherActor);
}