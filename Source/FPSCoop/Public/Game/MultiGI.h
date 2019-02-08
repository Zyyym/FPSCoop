// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Classes/FindSessionsCallbackProxy.h"
#include "MultiGI.generated.h"

/**
 * 
 */
UCLASS()
class FPSCOOP_API UMultiGI : public UGameInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated, BlueprintReadWrite, Category = Multiplayer)
		int32 MaxPlayers;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = Multiplayer)
		FText ServerName;

	UPROPERTY(BlueprintReadWrite, Category = Multiplayer)
		bool bIsMultiplayer;

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Multiplayer")
		void CreateLobby(const FText& LobbyName, int32 NewMaxPlayers, bool bLan);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Multiplayer")
		void JoinChoosenSession(FBlueprintSessionResult SessionToJoin);

	UFUNCTION(BlueprintImplementableEvent, Category = "Multiplayer")
		void DestroySession(APlayerController* Controller);

	UFUNCTION(BlueprintImplementableEvent, Category = "Multiplayer")
		void RestartLevel();

	UFUNCTION(BlueprintImplementableEvent, Category = "Multiplayer")
		void BackToMenu();

	UFUNCTION(BlueprintImplementableEvent, Category = "Multiplayer")
		void QuitTheGame();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Widgets")
		void ShowMenuWidget();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Widgets")
		void ShowHostWidget();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Widgets")
		void ShowFindGamesWidget();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Widgets")
		void ShowOptionsWidget();
};
