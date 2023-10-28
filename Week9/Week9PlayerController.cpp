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
}

void AWeek9PlayerController::BeginPlay()
{
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

