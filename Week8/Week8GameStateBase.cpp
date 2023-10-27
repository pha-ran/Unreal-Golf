// Fill out your copyright notice in the Description page of Project Settings.


#include "Week8GameStateBase.h"
#include "Week8PlayerState.h"
#include "Net/UnrealNetwork.h"

AWeek8GameStateBase::AWeek8GameStateBase()
{
	CurrentTurn = 0;
}

void AWeek8GameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeek8GameStateBase, CurrentTurn);
}

void AWeek8GameStateBase::SetCurrentTurn(int Turn)
{
	if (HasAuthority())
	{
		CurrentTurn = Turn;
	}
}

int AWeek8GameStateBase::GetCurrentTurn()
{
	return CurrentTurn;
}
