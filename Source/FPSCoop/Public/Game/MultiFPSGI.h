// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FPSCoop.h"
#include "Engine/GameInstance.h"
#include "MultiFPSGI.generated.h"

/**
 * 
 */
UCLASS()
class FPSCOOP_API UMultiFPSGI : public UGameInstance
{
	GENERATED_BODY()

/*protected:
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Server Settings")
		int32 MaxPlayers;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Server Settings")
		FString ServerName;

	TSharedPtr<class FOnlineSessionSettings> SessionSettings;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

public:
	UMultiFPSGI(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintImplementableEvent, Category = "Server Settings")
		void CreateGameSession(const FString& SessionName, bool bIsLAN, int32 MaxNumPlayers);

	bool HostSession(FUniqueNetIdRepl UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers);

	void FindSessions(FUniqueNetIdRepl UserId, bool bIsLAN, bool bIsPresence);

	bool JoinSession(FUniqueNetIdRepl UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);

	UFUNCTION(BlueprintCallable, Category = "Network|Test")
		void StartOnlineGame();

	UFUNCTION(BlueprintCallable, Category = "Network|Test")
		void FindOnlineGames();

	UFUNCTION(BlueprintCallable, Category = "Network|Test")
		void JoinOnlineGame();

	UFUNCTION(BlueprintCallable, Category = "Network|Test")
		void DestroySessionAndLeaveGame();

	//UFUNCTION(BlueprintImplementableEvent, Category = "Server Settings")
		//void FindLobby(TArray<FOnlineSessionSearchResult>& FindedSessions);

	//UFUNCTION(BlueprintImplementableEvent, Category = "Server Settings")
		//void JoinSession(const FOnlineSessionSearchResult& SessionToJoin);

	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);

	void OnFindSessionsComplete(bool bWasSuccessful);

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;

	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;

	FDelegateHandle OnFindSessionsCompleteDelegateHandle;

	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

	FDelegateHandle OnJoinSessionCompleteDelegateHandle;

	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;

	FDelegateHandle OnDestroySessionCompleteDelegateHandle;*/
};
