// Fill out your copyright notice in the Description page of Project Settings.


#include "Week8PlayerState.h"
#include "Net/UnrealNetwork.h"

AWeek8PlayerState::AWeek8PlayerState()
{
	PlayerTurn = 0;
}

void AWeek8PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeek8PlayerState, PlayerTurn);
}

void AWeek8PlayerState::SetPlayerTurn(int Turn)
{
	if (HasAuthority())
	{
		PlayerTurn = Turn;
	}
}

int AWeek8PlayerState::GetPlayerTurn()
{
	return PlayerTurn;
}
