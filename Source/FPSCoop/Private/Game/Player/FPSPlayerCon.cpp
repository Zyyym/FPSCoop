// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSPlayerCon.h"
#include "UI/PlayerView.h"
#include "Components/HealthComponent.h"
#include "UI/DamageText.h"
#include "TimerManager.h"
#include "FPSCoopCharacter.h"
#include "Menu/InGameMenu.h"
#include "Game/MultiplayerGM.h"
#include "Kismet/GameplayStatics.h"
#include "Menu/GameOverMenu.h"

void AFPSPlayerCon::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("OpenInGameMenu", IE_Pressed, this, &AFPSPlayerCon::InGameMenuOpened);
}


void AFPSPlayerCon::CreatePlayerView()
{
	if (PlayerViewClass && IsLocalController() && !PlayerView)
	{	
		PlayerView = CreateWidget<UPlayerView>(this, PlayerViewClass);
		PlayerView->AddToViewport();
		PlayerView->AssingHealthComp(GetPawn()->FindComponentByClass<UHealthComponent>());
		if (Role != ROLE_Authority)
			PlayerView->RefreshWeaponBar(Cast<AFPSCoopCharacter>(GetPawn())->GetWeapons()); //Nie dzia³a na serwerze
	} 
}


void AFPSPlayerCon::AssingHealthComp(UHealthComponent* NewComp)
{
	if (PlayerView)
	{
		PlayerView->AssingHealthComp(NewComp);
		if(Role != ROLE_Authority)
			PlayerView->RefreshWeaponBar(Cast<AFPSCoopCharacter>(GetPawn())->GetWeapons());
	}
}


void AFPSPlayerCon::ForceWidgetRefresh()
{
	if (Role != ROLE_Authority)
		PlayerView->RefreshWeaponBar(Cast<AFPSCoopCharacter>(GetPawn())->GetWeapons());
}


void AFPSPlayerCon::BeginPlayingState()
{
	Super::BeginPlayingState();
	if(!IsLocalController())
		return;
	CreatePlayerView();
	FInputModeGameOnly Mode;
	SetInputMode(Mode);
}

void AFPSPlayerCon::CreateWeaponBar(TArray<AWeapon*> Weapons)
{
	if (!PlayerView)
		CreatePlayerView();
	if(PlayerView)
		PlayerView->RefreshWeaponBar(Weapons); //Dzia³a na serwerze
}


void AFPSPlayerCon::ShowDamageText(int32 Damage, FVector OnLocation, bool bIsHeal)
{
	if(Role == ROLE_Authority)
		ClientShowDamageText(Damage, OnLocation, bIsHeal);
}


void AFPSPlayerCon::ClientShowDamageText_Implementation(int32 Damage, FVector OnLocation, bool bIsHeal)
{
	if (!DamageTextClass)
		return;

	FVector2D ScreenLocation;
	if (ProjectWorldLocationToScreen(OnLocation, ScreenLocation, true))
	{
		UDamageText* DamageTextWidget = CreateWidget<UDamageText>(this, DamageTextClass);
		DamageTextWidget->SetPosition(OnLocation, this);

		if (bIsHeal)
			DamageTextWidget->ShowHealText(Damage);
		else
			DamageTextWidget->ShowDamageText(Damage);
	}
}


void AFPSPlayerCon::ClientUpdateMisionStatus_Implementation(bool bHaveObjective)
{
	if (PlayerView)
		PlayerView->UdpateObjectiveText(bHaveObjective);
}


void AFPSPlayerCon::InGameMenuOpened()
{
	if (InGameMenu && InGameMenu->IsInViewport())
	{
		InGameMenuClosed();
	}
	else if (InGameMenuClass)
	{
		InGameMenu = CreateWidget<UInGameMenu>(this, InGameMenuClass);
		InGameMenu->AddToViewport();
		bShowMouseCursor = true;
		bEnableClickEvents = true;
		bEnableMouseOverEvents = true;
		FInputModeUIOnly Mode;
		SetInputMode(Mode);
	}
}


void AFPSPlayerCon::InGameMenuClosed()
{
	bShowMouseCursor = false;
	bEnableClickEvents = false;
	bEnableMouseOverEvents = false;
	InGameMenu->RemoveFromParent();
	FInputModeGameOnly Mode;
	SetInputMode(Mode);
}


void AFPSPlayerCon::AfterBlurout()
{
	if (BluroutEffect)
		BluroutEffect->RemoveFromParent();
	LookForActorToObserving();
}


void AFPSPlayerCon::PlayerStartOberving(APlayerController* NewObserver)
{
	if (NewObserver)
		Observers.Add(NewObserver);
}


void AFPSPlayerCon::LookForActorToObserving_Implementation()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AFPSPlayerCon* Cont = Cast<AFPSPlayerCon>(It->Get());
		if (Cont && Cont != this && !Cont->bIsObserving)
		{
			StartSpectating(Cont->GetPawn());
			Cont->PlayerStartOberving(this);
			AFPSPlayerCon* PC;
			for (APlayerController* PCon : Observers)
			{
				PC = Cast<AFPSPlayerCon>(PCon);
				if (PC)
				{
					PC->StartSpectating(Cont->GetPawn());
					Cont->PlayerStartOberving(PC);
				}
			}
			return;
		}	
	}
	AMultiplayerGM* MGM = Cast<AMultiplayerGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (MGM)
	{
		MGM->GameOver(false);
	}
}


bool AFPSPlayerCon::LookForActorToObserving_Validate()
{
	return true;
}



void AFPSPlayerCon::ShowGameOverWidget_Implementation(bool bWin)
{
	GameOverWidget = CreateWidget<UGameOverMenu>(this, GameOverClass);
	GameOverWidget->SetStatus(bWin);
	GameOverWidget->AddToViewport();
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	FInputModeUIOnly Mode;
	SetInputMode(Mode);
}


void AFPSPlayerCon::Blurout_Implementation()
{
	BluroutEffect = CreateWidget<UUserWidget>(this, BluroutEffectClass);
	BluroutEffect->AddToViewport();
	if (PlayerView)
		PlayerView->RemoveFromParent();
	FTimerHandle BluroutTimer;
	GetWorld()->GetTimerManager().SetTimer(BluroutTimer, this, &AFPSPlayerCon::AfterBlurout, 1);
}


void AFPSPlayerCon::StartSpectating(APawn* NewViewTarget)
{
	bIsObserving = true;
	if (!NewViewTarget)
	{
		ShowGameOverWidget(false);
		return;
	}
	SetViewTargetWithBlend(NewViewTarget);
}