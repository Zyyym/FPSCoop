// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ActionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class FPSCOOP_API UActionLibrary : public UObject
{
	GENERATED_BODY()
	
public:
	static void DealDamage(AActor* Target, int32 Damage);

	static void AddRadialImpulse(AActor* Target, FVector Location, float ImpulseSize, float ImpulsePower);

	static void AddRadialImpulseToComponents(FVector Location, float ImpulseSize, float ImpulsePower, TArray<AActor*> Ignore);

	static void AddRadialImpulseToPawns(FVector Location, float ImpulseSize, float ImpulsePower, TArray<AActor*> Ignore);

	static void AddRadialImpulseToPawns(FVector Location, float ImpulseSize, float ImpulsePower, TArray<AActor*> Ignore, int32 Damage);

	static bool IsTargetBlocked(UPrimitiveComponent* Component, FVector EndLocation, TArray<AActor*> Ignore);

	static bool IsTargetBlocked(AActor* Actor, FVector EndLocation, TArray<AActor*> Ignore);
};
