// Fill out your copyright notice in the Description page of Project Settings.


#include "Week7TestPlayerState.h"
#include "Net/UnrealNetwork.h"

void AWeek7TestPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeek7TestPlayerState, PlayerCode);
}

void AWeek7TestPlayerState::SetPlayerCode(FString playerCode)
{
	PlayerCode = playerCode;
}

FString AWeek7TestPlayerState::GetPlayerCode()
{
	return PlayerCode;
}
