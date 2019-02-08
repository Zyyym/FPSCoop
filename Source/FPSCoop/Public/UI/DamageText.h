// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageText.generated.h"

class UTextBlock;
UCLASS()
class FPSCOOP_API UDamageText : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UTextBlock* DamageTextBlock;

	UFUNCTION(BlueprintImplementableEvent, Category = "DamageText")
		void PushSetBlock();

	UFUNCTION(BlueprintCallable, Category = "DamageText")
		void SetBlock(UTextBlock* NewDamageTextBlock);	

	FVector2D PositionOfset;

	FVector WorldLocation;

	UFUNCTION()
		void SelfRemove();

	APlayerController* DisplayedBy;

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void ShowDamageText(int32 Damage);

	void ShowHealText(int32 Heal);

	void SetPosition(FVector NewWorldLocation, APlayerController* ByController);
};
