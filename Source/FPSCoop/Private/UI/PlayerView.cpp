// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerView.h"
#include "HealthBar.h"
#include "WeaponsBar.h"

void UPlayerView::RefreshHealthBar()
{
	HealthBar->RefreshBar();
}


void UPlayerView::AssingHealthComp(UHealthComponent* NewComp)
{
	HealthBar->AssingComp(NewComp);
}


void UPlayerView::RefreshWeaponBar(TArray<AWeapon*> Weapons)
{
	if (WeaponBar)
		WeaponBar->CreateBar(Weapons);
}