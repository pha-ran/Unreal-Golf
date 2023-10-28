// Fill out your copyright notice in the Description page of Project Settings.


#include "Week9PlayerState.h"
#include "Net/UnrealNetwork.h"

AWeek9PlayerState::AWeek9PlayerState()
{
	PlayerTurn = 0;
}

void AWeek9PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeek9PlayerState, PlayerTurn);
}

void AWeek9PlayerState::SetPlayerTurn(int Turn)
{
	if (HasAuthority())
	{
		PlayerTurn = Turn;
	}
}

int AWeek9PlayerState::GetPlayerTurn()
{
	return PlayerTurn;
}
