// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Week7TestPlayerController.generated.h"

UCLASS()
class WEEK7TEST_API AWeek7TestPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AWeek7TestPlayerController();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	TSubclassOf<class UDebugUserWidget> WIdgetBlueprint;
	class UDebugUserWidget* DebugUserWidget;

private:
	bool Initialized;
};
