// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthBar.h"
#include "Components/HealthComponent.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"
#include "WidgetTree.h"

void UHealthBar::RefreshBar()
{
	if (!HealthComp || !Bar)
		return;

	UImage* Temp;
	if(ActualHealth < HealthComp->GetHealth())
		for (int i = ActualHealth; i < HealthComp->GetHealth(); i++)
		{
			Temp = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
			Temp->SetBrush(HealthIcon);
			Bar->AddChild(Temp);
		}
	else
		for (int i = ActualHealth; i > HealthComp->GetHealth(); i--)
		{
			Bar->RemoveChildAt(Bar->GetChildrenCount() - 1);
		}
	ActualHealth = HealthComp->GetHealth();
}


void UHealthBar::AssingComp(UHealthComponent* NewHealth)
{
	ActualHealth = 0;
	if (Bar)
		Bar->ClearChildren();
	HealthComp = NewHealth;
	if (HealthComp)
		HealthComp->SetHealthBar(this);
	RefreshBar();
}