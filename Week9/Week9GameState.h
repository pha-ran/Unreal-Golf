// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Week9GameState.generated.h"

UCLASS()
class WEEK9_API AWeek9GameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AWeek9GameState();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetCurrentTurn(int Turn);

	int GetCurrentTurn();

private:
	UPROPERTY(Replicated)
	int CurrentTurn;
};
