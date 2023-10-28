// Fill out your copyright notice in the Description page of Project Settings.


#include "Week9PlayerController.h"
#include "ClientUserWidget.h"
#include "Week9Character.h"
#include "Week9GameMode.h"
#include "Week9PlayerState.h"
#include "UObject/ConstructorHelpers.h"

AWeek9PlayerController::AWeek9PlayerController()
{
	static ConstructorHelpers::FClassFinder<UClientUserWidget> _ClientWidgetBlueprint(TEXT("/Game/Blueprints/ClientWidgetBlueprint"));

	if (_ClientWidgetBlueprint.Class != NULL)
	{
		ClientWidgetBlueprint = _ClientWidgetBlueprint.Class;
	}
}

void AWeek9PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!HasAuthority() && MyPawn != nullptr && MyGameState != nullptr && ClientUserWidget != nullptr)
	{
		AWeek9PlayerState* WPS = Cast<AWeek9PlayerState>(PlayerState);

		if (WPS != nullptr)
		{
			ClientUserWidget->SetMyNameText(WPS->GetPlayerName());
		}

		ClientUserWidget->SetMyAngleText(MyPawn->GetAngle());
		ClientUserWidget->SetMySpeedText(MyPawn->GetSpeed());

		ClientUserWidget->SetCurrentTurnText(MyGameState->GetCurrentTurn());
		
		MyPlayerArray = MyGameState->PlayerArray;

		int Index = 0;

		for (APlayerState* MyPlayer : MyPlayerArray)
		{
			WPS = Cast<AWeek9PlayerState>(MyPlayer);

			if (WPS != nullptr && WPS->GetPlayerTurn() > 0)
			{
				if (Index == 0)
				{
					ClientUserWidget->SetPlayer1Text(WPS->GetPlayerTurn(), WPS->GetPlayerName());
				}
				else if (Index == 1)
				{
					ClientUserWidget->SetPlayer2Text(WPS->GetPlayerTurn(), WPS->GetPlayerName());
				}
				else if (Index == 2)
				{
					ClientUserWidget->SetPlayer3Text(WPS->GetPlayerTurn(), WPS->GetPlayerName());
				}
				else if (Index == 3)
				{
					ClientUserWidget->SetPlayer4Text(WPS->GetPlayerTurn(), WPS->GetPlayerName());
				}

				Index++;
			}
		}
	}
}

void AWeek9PlayerController::BeginPlay()
{
	MyPawn = Cast<AWeek9Character>(GetPawn());
	MyGameState = Cast<AWeek9GameState>(GetWorld()->GetGameState());

	if (!HasAuthority())
	{
		if (ClientWidgetBlueprint != nullptr)
		{
			ClientUserWidget = CreateWidget<UClientUserWidget>(this, ClientWidgetBlueprint);

			ClientUserWidget->AddToViewport();
		}
	}
}

