// Fill out your copyright notice in the Description page of Project Settings.

#include "DirectProjectile.h"
#include "Library/ActionLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"

ADirectProjectile::ADirectProjectile()
{
	Damage = 1;
	GetProjectileMovement()->InitialSpeed = 6000;
	GetProjectileMovement()->MaxSpeed = 6000;
	HitEffectScale = FVector(0.1f, 0.1f, 0.1f);
	DestroyEffectScale = FVector(0.1f, 0.1f, 0.1f);
}

void ADirectProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == GetOwner() || OtherActor == GetOwner()->GetOwner())
		return;
	if(Role == ROLE_Authority)
		UActionLibrary::DealDamage(OtherActor, Damage);
	Kill();
}