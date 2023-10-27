// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Week7TestGameMode.generated.h"

UCLASS(minimalapi)
class AWeek7TestGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AWeek7TestGameMode();
	virtual void OnPostLogin(AController* NewPlayer) override;
};
