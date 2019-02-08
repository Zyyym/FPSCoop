// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClickSygnature);

class UBorder;
UCLASS()
class FPSCOOP_API UMenuButton : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, Category = Button)
		UWidgetAnimation* MouseEnterAnim;

public:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseEnter(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent & InMouseEvent) override;

	UPROPERTY(BlueprintAssignable)
		FOnClickSygnature OnClick;
};
