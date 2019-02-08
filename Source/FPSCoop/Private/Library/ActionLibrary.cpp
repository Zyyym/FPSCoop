// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionLibrary.h"
#include "GameFramework/Actor.h"
#include "Components/HealthComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/MovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"


void UActionLibrary::DealDamage(AActor* Target, int32 Damage)
{
	if (!Target)
		return;
	UHealthComponent* HealthComp = Target->FindComponentByClass<UHealthComponent>();

	if (HealthComp)
	{
		HealthComp->DealDamage(Damage);
	}
}


void UActionLibrary::AddRadialImpulse(AActor* Target, FVector Location, float ImpulseSize, float ImpulsePower)
{
	if (!Target)
		return;
	UMovementComponent* MovementComp = Target->FindComponentByClass<UMovementComponent>();

	if (MovementComp)
	{
		MovementComp->AddRadialImpulse(Location, ImpulseSize, ImpulsePower, ERadialImpulseFalloff::RIF_Constant, true);
	}
}


void UActionLibrary::AddRadialImpulseToComponents(FVector Location, float ImpulseSize, float ImpulsePower, TArray<AActor*> Ignore)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_PhysicsBody));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	TArray<UPrimitiveComponent*> OverlapingComponents;
	UKismetSystemLibrary::SphereOverlapComponents(Ignore[0]->GetWorld(), Location, ImpulseSize, ObjectTypes, UPrimitiveComponent::StaticClass(), Ignore, OverlapingComponents);
	for (UPrimitiveComponent* Comp : OverlapingComponents)
	{	
		if (Comp->IsSimulatingPhysics() && UActionLibrary::IsTargetBlocked(Comp, Location, Ignore))
		{
			Comp->AddRadialImpulse(Location, ImpulseSize, ImpulsePower, ERadialImpulseFalloff::RIF_Constant, true);
		}
	}
}


void UActionLibrary::AddRadialImpulseToPawns(FVector Location, float ImpulseSize, float ImpulsePower, TArray<AActor*> Ignore)
{
	if (Ignore.Num() == 0)
		return;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	TArray<AActor*> OverlapingActors;
	UKismetSystemLibrary::SphereOverlapActors(Ignore[0]->GetWorld(), Location, ImpulseSize, ObjectTypes, AActor::StaticClass(), Ignore, OverlapingActors);

	for (AActor* Act : OverlapingActors)
	{
		if (Ignore[0]->GetClass() == Act->GetClass())
			return;
		if(UActionLibrary::IsTargetBlocked(Act, Location, Ignore))
			UActionLibrary::AddRadialImpulse(Act, Location, ImpulseSize, ImpulsePower);
	}
}


void UActionLibrary::AddRadialImpulseToPawns(FVector Location, float ImpulseSize, float ImpulsePower, TArray<AActor*> Ignore, int32 Damage)
{
	if (Ignore.Num() == 0)
		return;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	TArray<AActor*> OverlapingActors;
	UKismetSystemLibrary::SphereOverlapActors(Ignore[0]->GetWorld(), Location, ImpulseSize, ObjectTypes, AActor::StaticClass(), Ignore, OverlapingActors);

	for (AActor* Act : OverlapingActors)
	{
		if (Ignore[0]->GetClass() == Act->GetClass())
			return;
		if (UActionLibrary::IsTargetBlocked(Act, Location, Ignore))
		{
			UActionLibrary::DealDamage(Act, Damage);
			UActionLibrary::AddRadialImpulse(Act, Location, ImpulseSize, ImpulsePower);
		}
	}
}


bool UActionLibrary::IsTargetBlocked(UPrimitiveComponent* Component, FVector StartLocation, TArray<AActor*> Ignore)
{
	FVector EndLocation = Component->GetComponentLocation();
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActors(Ignore);
	Ignore[0]->GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECollisionChannel::ECC_Camera, Params);

	return Hit.GetComponent() == Component;
}


bool UActionLibrary::IsTargetBlocked(AActor* Actor, FVector EndLocation, TArray<AActor*> Ignore)
{
	FVector StartLocation = Actor->GetActorLocation();
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActors(Ignore);
	Ignore[0]->GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECollisionChannel::ECC_Camera, Params);

	return Hit.GetActor() == Actor;
}