// Fill out your copyright notice in the Description page of Project Settings.

#include "AmmoWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Projectiles/AmmoType.h"

void UAmmoWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Refresh();
}

void UAmmoWidget::SetDefaults(UTextBlock* NewAmmoText, UImage* NewImage, UWidgetAnimation* NewPrimaryAnim)
{
	AmmoText = NewAmmoText;
	Image = NewImage;
	PrimaryAnim = NewPrimaryAnim;
}

void UAmmoWidget::SetAmmo(UAmmoType* NewAmmo)
{
	if (Ammo)
		Ammo->SetWidget(nullptr);
	Ammo = NewAmmo;
	if (Ammo)
	{
		Ammo->SetWidget(this);
	}
	Refresh();
}

void UAmmoWidget::Refresh()
{
	if (!Ammo || !Image || !AmmoText)
		return;

	AmmoText->SetText(FText::AsNumber(Ammo->GetAmmo()));
	Image->SetBrush(Ammo->GetImage());
}


void UAmmoWidget::MakePrimary()
{
	PlayAnimation(PrimaryAnim);
}


void UAmmoWidget::MakeSecondary()
{
	PlayAnimation(PrimaryAnim, 0.f, 1, EUMGSequencePlayMode::Reverse);
}