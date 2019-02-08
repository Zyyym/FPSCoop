// Fill out your copyright notice in the Description page of Project Settings.

#include "AmmoBar.h"
#include "Components/HorizontalBox.h"
#include "UI/AmmoWidget.h"
#include "WidgetTree.h"
#include "Animation/WidgetAnimation.h"
void UAmmoBar::NativeConstruct()
{
	SetBar();
	Super::NativeConstruct();
}


void UAmmoBar::SetAmmoBarDefaults(UHorizontalBox* NewMyAmmoBar, UWidgetAnimation* NewShowAnim)
{
	MyAmmoBar = NewMyAmmoBar;
	ShowAnim = NewShowAnim;
}


void UAmmoBar::CreateBar(TArray<UAmmoType*> Ammos)
{
	if (!MyAmmoBar)
		SetBar();

	if (!MyAmmoBar || !AmmoWidgetClass)
		return;
	bFirstSet = false;
	UAmmoWidget* Temp;
	for (UAmmoType* Am : Ammos)
	{
		Temp = WidgetTree->ConstructWidget<UAmmoWidget>(AmmoWidgetClass);
		AmmoWidgets.Add(Temp);
		MyAmmoBar->AddChild(Temp);
		Temp->SetAmmo(Am);
	}
}


void UAmmoBar::PlayShowAnimation()
{
	SetVisibility(ESlateVisibility::Visible);
	if (!bFirstSet)
	{
		AmmoWidgets[0]->MakePrimary();
		for (int i = 1; i < AmmoWidgets.Num(); i++)
			AmmoWidgets[i]->MakeSecondary();
		bFirstSet = true;
	}

	ShowAnim->OnAnimationFinished.Remove(this, FName("Hide"));
	PlayAnimation(ShowAnim);
}


void UAmmoBar::PlayHideAnimation()
{
	ShowAnim->OnAnimationFinished.AddDynamic(this, &UAmmoBar::Hide);
	PlayAnimation(ShowAnim, 0.f, 1, EUMGSequencePlayMode::Reverse);
}


void UAmmoBar::Hide()
{
	SetVisibility(ESlateVisibility::Hidden);
	//RemoveFromParent();
}