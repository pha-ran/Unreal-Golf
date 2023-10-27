// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Week7TestPlayerState.generated.h"

UCLASS()
class WEEK7TEST_API AWeek7TestPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void SetPlayerCode(FString playerCode);
	FString GetPlayerCode();

private:
	UPROPERTY(Replicated)
	FString PlayerCode;
};
