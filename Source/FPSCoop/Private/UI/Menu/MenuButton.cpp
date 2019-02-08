// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuButton.h"


FReply UMenuButton::NativeOnMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonUp(Geometry, InMouseEvent);
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		OnClick.Broadcast();
	}
	return FReply::Handled();
}


void UMenuButton::NativeOnMouseEnter(const FGeometry & InGeometry, const FPointerEvent& InMouseEvent)
{
	if (MouseEnterAnim)
		PlayAnimation(MouseEnterAnim);
}


void UMenuButton::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	if (MouseEnterAnim)
		PlayAnimation(MouseEnterAnim, 0, 1, EUMGSequencePlayMode::Reverse);
}