// Copyright Epic Games, Inc. All Rights Reserved.

#include "Week10GameMode.h"
#include "Week10Character.h"
#include "Week10GameState.h"
#include "Week10PlayerController.h"
#include "Week10PlayerState.h"
#include "UObject/ConstructorHelpers.h"

AWeek10GameMode::AWeek10GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	GameStateClass = AWeek10GameState::StaticClass();
	PlayerControllerClass = AWeek10PlayerController::StaticClass();
	PlayerStateClass = AWeek10PlayerState::StaticClass();
}
