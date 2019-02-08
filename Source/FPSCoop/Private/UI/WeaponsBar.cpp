
// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponsBar.h"
#include "Components/HorizontalBox.h"
#include "UI/WeaponWidget.h"
#include "WidgetTree.h"

void UWeaponsBar::CreateBar(TArray<AWeapon*> Weapons)
{
	if (!Bar || !WeaponWidgetClass)
		return;
	Bar->ClearChildren();
	WaeponWidgets.Empty();
	int8 i = 0;
	UWeaponWidget* Temp;
	for (AWeapon* Am : Weapons)
	{
		Temp = WidgetTree->ConstructWidget<UWeaponWidget>(WeaponWidgetClass);
		WaeponWidgets.Add(Temp);
		Bar->AddChild(Temp);
		Temp->SetWeapon(Am);
		if (i > 0)
			Temp->MakeSecondary();
		i++;
	}
}