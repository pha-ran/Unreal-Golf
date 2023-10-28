// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Week9GameState.h"
#include "Week9Character.h"
#include "Week9PlayerController.generated.h"

UCLASS()
class WEEK9_API AWeek9PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AWeek9PlayerController();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	TSubclassOf<class UClientUserWidget> ClientWidgetBlueprint;

	class UClientUserWidget* ClientUserWidget;

	AWeek9Character* MyPawn;

	AWeek9GameState* MyGameState;

	TArray<APlayerState*> MyPlayerArray;
};
