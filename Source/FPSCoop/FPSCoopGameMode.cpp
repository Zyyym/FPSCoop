// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "FPSCoopGameMode.h"
#include "FPSCoopHUD.h"
#include "FPSCoopCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFPSCoopGameMode::AFPSCoopGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSCoopHUD::StaticClass();
}
