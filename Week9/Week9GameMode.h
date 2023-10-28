// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Week9GameState.h"
#include "GameFramework/GameModeBase.h"
#include "Week9GameMode.generated.h"

UCLASS(minimalapi)
class AWeek9GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AWeek9GameMode();

	virtual void OnPostLogin(AController* NewPlayer) override;

	void NextTurn();

protected:
	virtual void BeginPlay() override;

	AWeek9GameState* MyGameState;

	int MaxTurn;

};



