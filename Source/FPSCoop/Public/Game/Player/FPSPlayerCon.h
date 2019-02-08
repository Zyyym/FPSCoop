// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPSPlayerCon.generated.h"

class UPlayerView;
class AWeapon;
class UDamageText;
UCLASS()
class FPSCOOP_API AFPSPlayerCon : public APlayerController
{
	GENERATED_BODY()

protected:
	UPlayerView* PlayerView;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayerView)
		TSubclassOf<UPlayerView> PlayerViewClass;

	virtual void BeginPlayingState() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Health)
		TSubclassOf<UDamageText> DamageTextClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UInGameMenu> InGameMenuClass;

	UInGameMenu* InGameMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		TSubclassOf<UUserWidget> BluroutEffectClass;

	UUserWidget* BluroutEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UGameOverMenu> GameOverClass;

	UGameOverMenu* GameOverWidget;

	UFUNCTION()
		void AfterBlurout();

	UFUNCTION(Server, Reliable, WithValidation)
		void LookForActorToObserving();

	bool bIsObserving;

	TArray<APlayerController*> Observers;
	
public:
	void ForceWidgetRefresh();

	virtual void SetupInputComponent() override;

	void CreatePlayerView();

	void AssingHealthComp(class UHealthComponent* NewComp);

	void CreateWeaponBar(TArray<AWeapon*> Weapons);

	void ShowDamageText(int32 Damage, FVector OnLocation, bool bIsHeal = false);

	UFUNCTION(Client, Reliable)
		void ClientShowDamageText(int32 Damage, FVector OnLocation, bool bIsHeal = false);

	UFUNCTION(Client, Reliable)
		void ClientUpdateMisionStatus(bool bHaveObjective);

	void InGameMenuOpened();

	void InGameMenuClosed();

	UFUNCTION(Client, Reliable)
		void Blurout();

	void StartSpectating(APawn* NewViewTarget);

	void PlayerStartOberving(APlayerController* NewObserver);

	UFUNCTION(Client, Reliable)
		void ShowGameOverWidget(bool bWin);
};
