// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Classes/FindSessionsCallbackProxy.h"
#include "SessionResultWidget.generated.h"

class UTextBlock;
UCLASS()
class FPSCOOP_API USessionResultWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	FBlueprintSessionResult Session;

	UTextBlock* SessionNameText;

	UTextBlock* SessionPlayersText;

public:
	UFUNCTION(BlueprintCallable, Category = "Multiplayer")
		void SetResultWidget(FBlueprintSessionResult NewSession, const FString& SessionName, const int32 Players, const int32 MaxPlayers);

	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent) override;
};
