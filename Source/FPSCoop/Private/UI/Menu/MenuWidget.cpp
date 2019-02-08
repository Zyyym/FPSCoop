// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuWidget.h"
#include "Menu/MenuButton.h"
#include "Game/MultiGI.h"

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (HostGameButton)
		HostGameButton->OnClick.AddDynamic(this, &UMenuWidget::HostGame);

	if (FindGameButton)
		FindGameButton->OnClick.AddDynamic(this, &UMenuWidget::FindGames);

	if (OptionsButton)
		OptionsButton->OnClick.AddDynamic(this, &UMenuWidget::OpenOptions);
}


void UMenuWidget::HostGame()
{
	UMultiGI* GI = Cast<UMultiGI>(GetWorld()->GetGameInstance());
	if (GI)
	{
		GI->ShowHostWidget();
	}
}


void UMenuWidget::FindGames()
{
	UMultiGI* GI = Cast<UMultiGI>(GetWorld()->GetGameInstance());
	if (GI)
		GI->ShowFindGamesWidget();
}


void UMenuWidget::OpenOptions()
{
	UMultiGI* GI = Cast<UMultiGI>(GetWorld()->GetGameInstance());
	if (GI)
		GI->ShowOptionsWidget();
}


void UMenuWidget::SetButtons(UMenuButton* NewHostGameButton, UMenuButton* NewFindGameButton, UMenuButton* NewOptionsButton, UMenuButton* NewQuitGamaButton)
{
	HostGameButton = NewHostGameButton;
	FindGameButton = NewFindGameButton;
	OptionsButton = NewOptionsButton;
	QuitGamaButton = NewQuitGamaButton;
}