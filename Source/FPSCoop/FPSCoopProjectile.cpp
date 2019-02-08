// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "FPSCoopProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AFPSCoopProjectile::AFPSCoopProjectile() 
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AFPSCoopProjectile::OnHit);		// set up a notification for when this component hits something blocking

	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	HitEffectScale = FVector(1.f, 1.f, 1.f);
	DestroyEffectScale = FVector(1.f, 1.f, 1.f);

	SetReplicates(true);
	SetReplicateMovement(true);
	MinNetUpdateFrequency = 33.3f;
	NetUpdateFrequency = 66.6f;
}

void AFPSCoopProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	/*if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		if(Role == ROLE_Authority)
			Destroy();
	}*/

	if (HitEffect)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, Hit.ImpactPoint, FRotator(0.f, 0.f, 0.f), HitEffectScale);
}


void AFPSCoopProjectile::Kill()
{
	if(DestroyEffect)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestroyEffect, GetActorLocation(), FRotator(0.f, 0.f, 0.f), DestroyEffectScale);
	
	if (Role == ROLE_Authority)
	{
		Destroy();
	}
}