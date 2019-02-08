// Fill out your copyright notice in the Description page of Project Settings.

#include "ImpulseProjectile.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "Library/ActionLibrary.h"
#include "GameFramework/MovementComponent.h"


AImpulseProjectile::AImpulseProjectile()
{
	Damage = 0;
	ImpulsePower = 1000.f;
	ImpulseSize = 500.f;
}


void AImpulseProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
	Kill();
}


void AImpulseProjectile::Kill()
{
	if (Role == ROLE_Authority)
	{
		TArray<AActor*> Ignore;
		Ignore.Add(this);

		UActionLibrary::AddRadialImpulseToComponents(GetActorLocation(), ImpulseSize, ImpulsePower, Ignore);

		if (Damage > 0)
			UActionLibrary::AddRadialImpulseToPawns(GetActorLocation(), ImpulseSize, ImpulsePower, Ignore, Damage);
		else
			UActionLibrary::AddRadialImpulseToPawns(GetActorLocation(), ImpulseSize, ImpulsePower, Ignore);
	}
	
	Super::Kill();
}