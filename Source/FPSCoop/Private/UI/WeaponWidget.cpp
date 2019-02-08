// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Weapon/Weapon.h"
#include "Weapon/ProjectileWeapon.h"
#include "Components/VerticalBox.h"
#include "UI/AmmoBar.h"
#include "Player/FPSPlayerCon.h"
#include "TimerManager.h"

void UWeaponWidget::SetDefaults(UTextBlock* NewAmmoText, UImage* NewImage, UVerticalBox* NewVerBox, UWidgetAnimation* NewPrimaryAnim, UWidgetAnimation* NewUsableAnim)
{
	AmmoText = NewAmmoText;
	Image = NewImage;
	VerBox = NewVerBox;
	PrimaryAnim = NewPrimaryAnim;
	MakeUsableAnim = NewUsableAnim;
}


void UWeaponWidget::SetWeapon(AWeapon* NewWeapon)
{
	if (Weapon)
		Weapon->SetWidget(nullptr);

	if (!NewWeapon)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UWeaponWidget::ForceRefresh);
		return;
	}		
	Weapon = NewWeapon;
	Weapon->SetWidget(this);
	Refresh();
	AProjectileWeapon* ProjectileWeapon = Cast<AProjectileWeapon>(Weapon);
	if (ProjectileWeapon && VerBox)
	{
		if (!MyAmmoBar)
			MyAmmoBar = ProjectileWeapon->GetAmmoBar();
		VerBox->AddChildToVerticalBox(MyAmmoBar);
		MyAmmoBar->SetVisibility(ESlateVisibility::Hidden);
	}
}


void UWeaponWidget::Refresh()
{
	if (!Weapon || !Image || !AmmoText)
		return;

	Image->SetBrush(Weapon->GetImage());
	AmmoText->SetVisibility(ESlateVisibility::Visible);
	const int32 Ammo = Weapon->GetAmmo();
	if (Ammo == -2)
	{
		AmmoText->SetText(FText::FromString(FString("M")));
		return;
	}
	if (Ammo == -1)
	{
		AmmoText->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	AmmoText->SetText(FText::AsNumber(Ammo));
}


void UWeaponWidget::ForceRefresh()
{
	GetOwningPlayer<AFPSPlayerCon>()->ForceWidgetRefresh();
}


void UWeaponWidget::MakePrimary()
{
	PlayAnimation(PrimaryAnim);

	if (MyAmmoBar)
	{
		if (VerBox)
		{
			//VerBox->AddChildToVerticalBox(MyAmmoBar);
			MyAmmoBar->PlayShowAnimation();
		}
		return;
	}

	AProjectileWeapon* ProjectileWeapon = Cast<AProjectileWeapon>(Weapon);
	if (ProjectileWeapon && VerBox)
	{
		if(!MyAmmoBar)
			MyAmmoBar = ProjectileWeapon->GetAmmoBar();
		if (MyAmmoBar)
		{
			//VerBox->AddChildToVerticalBox(MyAmmoBar);
			MyAmmoBar->PlayShowAnimation();
		}
	}

}


void UWeaponWidget::MakeSecondary()
{
	PlayAnimation(PrimaryAnim, 0.f, 1, EUMGSequencePlayMode::Reverse);
	if (MyAmmoBar && VerBox)
	{
		MyAmmoBar->PlayHideAnimation();
	}
}


void UWeaponWidget::StartMakeUsable(float Time)
{
	PlayAnimation(MakeUsableAnim, 0, 1, EUMGSequencePlayMode::Forward, 1.f / Time);
}
