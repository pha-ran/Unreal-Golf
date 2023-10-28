// Copyright Epic Games, Inc. All Rights Reserved.

#include "Week9GameMode.h"
#include "Week9Character.h"
#include "Week9GameState.h"
#include "Week9PlayerController.h"
#include "Week9PlayerState.h"
#include "UObject/ConstructorHelpers.h"

AWeek9GameMode::AWeek9GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	GameStateClass = AWeek9GameState::StaticClass();
	PlayerStateClass = AWeek9PlayerState::StaticClass();
	PlayerControllerClass = AWeek9PlayerController::StaticClass();
	PlayerStateClass = AWeek9PlayerState::StaticClass();

	MaxTurn = 1;
}

void AWeek9GameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	if (MyGameState != nullptr)
	{
		if (MyGameState->PlayerArray.Num() == 4 && MyGameState->GetCurrentTurn() == 0)
		{
			NextTurn();
		}
	}

	AWeek9PlayerState* WPS = Cast<AWeek9PlayerState>(NewPlayer->PlayerState);

	if (WPS != nullptr)
	{
		WPS->SetPlayerTurn(MaxTurn);
		MaxTurn++;
	}
}

void AWeek9GameMode::NextTurn()
{
	if (MyGameState != nullptr)
	{
		// 입력 무시
		for (APlayerState* PS : MyGameState->PlayerArray)
		{
			AWeek9Character* WC = Cast<AWeek9Character>(PS->GetPawn());

			if (WC != nullptr)
			{
				WC->SetIgnoreInputAll(true);
			}
		}

		// 다음 턴 계산
		bool FinishTurn = true;

		for (APlayerState* PS : MyGameState->PlayerArray)
		{
			AWeek9Character* WC = Cast<AWeek9Character>(PS->GetPawn());
			AWeek9PlayerState* WPS = Cast<AWeek9PlayerState>(PS);

			if (WC != nullptr && WPS != nullptr)
			{
				if (MyGameState->GetCurrentTurn() < WPS->GetPlayerTurn())
				{
					MyGameState->SetCurrentTurn(WPS->GetPlayerTurn());
					WC->SetIgnoreInputAll(false);
					FinishTurn = false;
					break;
				}
			}
		}

		// 다음 루프 시작
		if (FinishTurn)
		{
			AWeek9Character* WC = Cast<AWeek9Character>(MyGameState->PlayerArray[0]->GetPawn());
			AWeek9PlayerState* WPS = Cast<AWeek9PlayerState>(MyGameState->PlayerArray[0]);

			if (WC != nullptr && WPS != nullptr)
			{
				MyGameState->SetCurrentTurn(WPS->GetPlayerTurn());
				WC->SetIgnoreInputAll(false);
			}
		}
	}
}

void AWeek9GameMode::BeginPlay()
{
	MyGameState = Cast<AWeek9GameState>(GameState);
}
