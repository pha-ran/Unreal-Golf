// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Week9PlayerState.generated.h"

UCLASS()
class WEEK9_API AWeek9PlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AWeek9PlayerState();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void SetPlayerTurn(int Turn);
	int GetPlayerTurn();

private:
	UPROPERTY(Replicated)
	int PlayerTurn;

};
