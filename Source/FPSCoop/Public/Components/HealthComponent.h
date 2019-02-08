// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKilledSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageTakenSignature, int32, Damage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealedSignature, int32, hEAL);

class UHealthBar;
class UDamageText;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSCOOP_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Health)
		int32 DefHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Health, ReplicatedUsing = OnRep_SetHealth)
		int32 Health;

	UFUNCTION()
		void OnRep_SetHealth();

	int32 MaxHealth;

	UHealthBar* HealthBar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Health)
		bool bShowDamage;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = Health)
		void DealDamage(int32 Damage);

	UFUNCTION(BlueprintCallable, Category = Health)
		void ApplyHeal(int32 Heal);

	int32 GetHealth() const { return Health; };

	void SetHealthBar(UHealthBar* NewBar) { HealthBar = NewBar; };

	void Kill();

	FOnKilledSignature OnKilled;

	FOnDamageTakenSignature OnDamageTaken;

	FOnHealedSignature OnHealed;
};
