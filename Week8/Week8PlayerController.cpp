// Fill out your copyright notice in the Description page of Project Settings.


#include "Week8PlayerController.h"
#include "Week8PlayerState.h"
#include "MainUserWidget.h"
#include "Week8Gamemode.h"
#include "Week8Character.h"
#include "UObject/ConstructorHelpers.h"

AWeek8PlayerController::AWeek8PlayerController()
{
	static ConstructorHelpers::FClassFinder<UMainUserWidget> MyWIdgetBlueprint(TEXT("/Game/ThirdPerson/Blueprints/MyWidgetBlueprint"));

	if (MyWIdgetBlueprint.Class != NULL)
	{
		MainUserWidgetBlueprint = MyWIdgetBlueprint.Class;
	}
}

void AWeek8PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HasAuthority())
	{
		if (MyGameState != nullptr)
		{
			MyPlayerArray = MyGameState->PlayerArray;

			FString MyPlayerTurn;
			FString MyPlayerName;
			FString MyUniqueId;
			
			MyPlayerTurn += FString::Printf(TEXT("%d"), MyGameState->GetCurrentTurn());

			for (APlayerState* MyPlayer : MyPlayerArray)
			{
				AWeek8PlayerState* WPS = Cast<AWeek8PlayerState>(MyPlayer);

				if (WPS != nullptr)
				{
					if (WPS->GetPlayerTurn() > 0)
					{
						MyPlayerTurn += FString::Printf(TEXT("\n%d"), WPS->GetPlayerTurn());
					}
					else
					{
						MyPlayerTurn += FString::Printf(TEXT("\n-1"));
					}
				}
				else
				{
					MyPlayerTurn += FString::Printf(TEXT("\n-1"));
				}

				MyPlayerName += "\n" + MyPlayer->GetPlayerName();
				MyUniqueId += "\n" + MyPlayer->GetUniqueId()->ToString();
			}

			MainUserWidget->SetPlayerText(MyPlayerTurn, MyPlayerName, MyUniqueId);
		}
	}
}

void AWeek8PlayerController::BeginPlay()
{
	MyGameState = Cast<AWeek8GameStateBase>(GetWorld()->GetGameState());

	if (HasAuthority())
	{
		UE_LOG(LogTemp, Display, TEXT("#################### Server PlayerController BeginPlay ####################"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "#################### Client PlayerController BeginPlay ####################");

		if (MainUserWidgetBlueprint != nullptr)
		{
			MainUserWidget = CreateWidget<UMainUserWidget>(this, MainUserWidgetBlueprint);

			MainUserWidget->AddToViewport();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, "#################### WIdgetBlueprint Is Null !!!!! ####################");
		}
	}
}
