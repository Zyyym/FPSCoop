// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AmmoBar.generated.h"

class UAmmoType;
class UHorizontalBox;
class UAmmoWidget;
UCLASS()
class FPSCOOP_API UAmmoBar : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AmmoBar")
		TSubclassOf<UAmmoWidget> AmmoWidgetClass;

	UAmmoWidget* ActualWidget;

	TArray<UAmmoWidget*> AmmoWidgets;

	UPROPERTY(BlueprintReadWrite, Category = "AmmoBar")
		UHorizontalBox* MyAmmoBar;

	UFUNCTION(BlueprintImplementableEvent, Category = "AmmoBar")
		void SetBar();

	UFUNCTION(BlueprintCallable, Category = "AmmoBar")
		void SetAmmoBarDefaults(UHorizontalBox* NewMyAmmoBar, UWidgetAnimation* NewShowAnim);

	UWidgetAnimation* ShowAnim;

	bool bFirstSet;

public:
	virtual void NativeConstruct() override;

	void CreateBar(TArray<UAmmoType*> Ammos);

	void PlayShowAnimation();

	void PlayHideAnimation();

	UFUNCTION()
		void Hide();
};
