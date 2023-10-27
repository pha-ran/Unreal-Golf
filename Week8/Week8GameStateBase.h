// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Week8GameStateBase.generated.h"

UCLASS()
class WEEK8_API AWeek8GameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	AWeek8GameStateBase();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void SetCurrentTurn(int Turn);
	int GetCurrentTurn();

private:
	UPROPERTY(Replicated)
	int CurrentTurn;
};
