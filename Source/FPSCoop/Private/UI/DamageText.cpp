// Fill out your copyright notice in the Description page of Project Settings.

#include "DamageText.h"
#include "Components/TextBlock.h"
#include "Math/Color.h"
#include "TimerManager.h"
#include "Engine/World.h"

void UDamageText::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	PositionOfset += FVector2D(0, -40) * InDeltaTime;
	FVector2D Location;
	if(DisplayedBy->ProjectWorldLocationToScreen(WorldLocation, Location, true))
		SetPositionInViewport(Location + PositionOfset);
}


void UDamageText::SetBlock(UTextBlock* NewDamageTextBlock)
{
	DamageTextBlock = NewDamageTextBlock;
}


void UDamageText::ShowDamageText(int32 Damage)
{
	if (!DamageTextBlock)
		return;
	
	DamageTextBlock->SetText(FText::AsNumber(Damage));
	DamageTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
}


void UDamageText::ShowHealText(int32 Heal)
{
	if (!DamageTextBlock)
		return;
	
	DamageTextBlock->SetText(FText::AsNumber(Heal));
	DamageTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor::Green));
}


void UDamageText::SelfRemove()
{
	RemoveFromParent();
}


void UDamageText::SetPosition(FVector NewWorldLocation, APlayerController* ByController)
{
	WorldLocation = NewWorldLocation;
	DisplayedBy = ByController;
	PositionOfset = FVector2D(FMath::RandRange(-20.f, 20.f), 0);
	AddToViewport();
	SetAlignmentInViewport(FVector2D(0.5f, 1));

	FTimerHandle DamageTextRemoveTimer;
	GetWorld()->GetTimerManager().SetTimer(DamageTextRemoveTimer, this, &UDamageText::SelfRemove, 1.f);
}