// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Week8GameStateBase.h"
#include "Week8PlayerController.generated.h"

UCLASS()
class WEEK8_API AWeek8PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AWeek8PlayerController();

	virtual void Tick(float DeltaTime) override;

	AWeek8GameStateBase* MyGameState;
	TArray<APlayerState*> MyPlayerArray;

protected:
	virtual void BeginPlay() override;
	TSubclassOf<class UMainUserWidget> MainUserWidgetBlueprint;
	class UMainUserWidget* MainUserWidget;
};
