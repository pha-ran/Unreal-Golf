// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Week8GameStateBase.h"
#include "GameFramework/GameModeBase.h"
#include "Week8GameMode.generated.h"

UCLASS(minimalapi)
class AWeek8GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AWeek8GameMode();

	virtual void OnPostLogin(AController* NewPlayer) override;

	void NextTurn();

	AWeek8GameStateBase* MyGameState;

protected:
	virtual void BeginPlay() override;

private:
	int MaxTurn;

};



