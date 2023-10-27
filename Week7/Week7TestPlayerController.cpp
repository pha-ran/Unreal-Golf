// Fill out your copyright notice in the Description page of Project Settings.


#include "Week7TestPlayerController.h"
#include "Week7TestPlayerState.h"
#include "DebugUserWidget.h"
#include "UObject/ConstructorHelpers.h"

AWeek7TestPlayerController::AWeek7TestPlayerController()
{
	static ConstructorHelpers::FClassFinder<UDebugUserWidget> NewWIdgetBlueprint(TEXT("/Game/ThirdPerson/Blueprints/NewWidgetBlueprint"));

	if (NewWIdgetBlueprint.Class != NULL)
	{
		WIdgetBlueprint = NewWIdgetBlueprint.Class;
	}

	Initialized = false;
}

void AWeek7TestPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (HasAuthority())
	{
		//
	}
	else
	{
		if (Initialized == false)
		{
			AWeek7TestPlayerState* week7TestPlayerState = Cast<AWeek7TestPlayerState>(PlayerState);

			if (week7TestPlayerState != nullptr)
			{
				DebugUserWidget->SetPlayerCodeText(*(week7TestPlayerState->GetPlayerCode()));

				Initialized = true;
			}
			else
			{
				DebugUserWidget->SetPlayerCodeText(TEXT("loading"));
			}

			//SetShowMouseCursor(true);
			//SetInputMode(FInputModeUIOnly());
		}
	}
}

void AWeek7TestPlayerController::BeginPlay()
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Display, TEXT("#################### Server PlayerController BeginPlay ####################"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "#################### Client PlayerController BeginPlay ####################");

		if (WIdgetBlueprint != nullptr)
		{
			DebugUserWidget = CreateWidget<UDebugUserWidget>(this, WIdgetBlueprint);

			DebugUserWidget->AddToViewport();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "#################### WIdgetBlueprint Is Null !!!!! ####################");
		}
	}
}
