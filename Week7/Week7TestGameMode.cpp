// Copyright Epic Games, Inc. All Rights Reserved.

#include "Week7TestGameMode.h"
#include "Week7TestGameState.h"
#include "Week7TestPlayerState.h"
#include "Week7TestPlayerController.h"
#include "Math/UnrealMathUtility.h"
#include "Week7TestCharacter.h"
#include "UObject/ConstructorHelpers.h"

AWeek7TestGameMode::AWeek7TestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	GameStateClass = AWeek7TestGameState::StaticClass();
	PlayerStateClass = AWeek7TestPlayerState::StaticClass();
	PlayerControllerClass = AWeek7TestPlayerController::StaticClass();

	UE_LOG(LogTemp, Display, TEXT("#################### GameMode Construct ####################"));
}

void AWeek7TestGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	if (NewPlayer != nullptr)
	{
		AWeek7TestPlayerState* PlayerState = Cast<AWeek7TestPlayerState>(NewPlayer->PlayerState);

		if (PlayerState != nullptr)
		{
			int32 Code = FMath::RandRange(100000, 999999);

			PlayerState->SetPlayerCode(FString::FromInt(Code));

			UE_LOG(LogTemp, Display, TEXT("#################### GameMode OnPostLogin : %s ####################"), *(PlayerState->GetPlayerCode()));
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("#################### GameMode OnPostLogin : PlayerState is not AWeek7TestPlayerState ####################"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("#################### GameMode OnPostLogin : Null ####################"));
	}
}
