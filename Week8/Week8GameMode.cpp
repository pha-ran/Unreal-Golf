// Copyright Epic Games, Inc. All Rights Reserved.

#include "Week8GameMode.h"
#include "Week8Character.h"
#include "Week8GameStateBase.h"
#include "Week8PlayerController.h"
#include "Week8PlayerState.h"
#include "UObject/ConstructorHelpers.h"

AWeek8GameMode::AWeek8GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	GameStateClass = AWeek8GameStateBase::StaticClass();
	PlayerStateClass = AWeek8PlayerState::StaticClass();
	PlayerControllerClass = AWeek8PlayerController::StaticClass();
	PlayerStateClass = AWeek8PlayerState::StaticClass();

	MaxTurn = 1;
}

void AWeek8GameMode::OnPostLogin(AController* NewPlayer)
{
	if (MyGameState != nullptr) {

		// 플레이어 목록 출력
		UE_LOG(LogTemp, Display, TEXT("#################### Start ####################"));

		for (TObjectPtr<APlayerState> player : MyGameState->PlayerArray)
		{
			UE_LOG(LogTemp, Display, TEXT("########## name : %s, id : %s ##########"), *player->GetPlayerName(), *player->GetUniqueId()->ToString());
		}

		UE_LOG(LogTemp, Display, TEXT("#################### End ####################"));

		// 4명 모이면 턴 부여
		if (MyGameState->PlayerArray.Num() == 4 && MyGameState->GetCurrentTurn() == 0)
		{
			NextTurn();
		}
	}

	AWeek8PlayerState* WPS = Cast<AWeek8PlayerState>(NewPlayer->PlayerState);

	if (WPS != nullptr)
	{
		WPS->SetPlayerTurn(MaxTurn);
		MaxTurn++;
	}
}

void AWeek8GameMode::NextTurn()
{
	if (MyGameState != nullptr)
	{
		// 입력 무시
		for (APlayerState* PS : MyGameState->PlayerArray)
		{
			AWeek8Character* WC = Cast<AWeek8Character>(PS->GetPawn());

			if (WC != nullptr)
			{
				WC->SetIgnoreInputAll(true);
			}
		}

		// 다음 턴 계산

		bool FinishTurn = true;

		for (APlayerState* PS : MyGameState->PlayerArray)
		{
			AWeek8Character* WC = Cast<AWeek8Character>(PS->GetPawn());
			AWeek8PlayerState* WPS = Cast<AWeek8PlayerState>(PS);

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

		if (FinishTurn)
		{
			AWeek8Character* WC = Cast<AWeek8Character>(MyGameState->PlayerArray[0]->GetPawn());
			AWeek8PlayerState* WPS = Cast<AWeek8PlayerState>(MyGameState->PlayerArray[0]);

			if (WC != nullptr && WPS != nullptr)
			{
				MyGameState->SetCurrentTurn(WPS->GetPlayerTurn());
				WC->SetIgnoreInputAll(false);
			}
		}
	}
}

void AWeek8GameMode::BeginPlay()
{
	MyGameState = Cast<AWeek8GameStateBase>(GameState);
}
