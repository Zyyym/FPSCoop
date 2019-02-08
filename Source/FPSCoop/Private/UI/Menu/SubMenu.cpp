// Fill out your copyright notice in the Description page of Project Settings.

#include "SubMenu.h"
#include "Game/MultiGI.h"

void USubMenu::BackToMenu()
{
	UMultiGI* GI = Cast<UMultiGI>(GetWorld()->GetGameInstance());
	if (GI)
		GI->ShowMenuWidget();
}