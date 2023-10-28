// Fill out your copyright notice in the Description page of Project Settings.


#include "Week9GameState.h"
#include "Week9PlayerState.h"
#include "Net/UnrealNetwork.h"

AWeek9GameState::AWeek9GameState()
{
	CurrentTurn = 0;
}

void AWeek9GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeek9GameState, CurrentTurn);
}

void AWeek9GameState::SetCurrentTurn(int Turn)
{
	if (HasAuthority())
	{
		CurrentTurn = Turn;
	}
}

int AWeek9GameState::GetCurrentTurn()
{
	return CurrentTurn;
}
