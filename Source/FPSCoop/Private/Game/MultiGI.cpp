// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiGI.h"
#include "Net/UnrealNetwork.h"



void UMultiGI::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMultiGI, MaxPlayers);
	DOREPLIFETIME(UMultiGI, ServerName);
}