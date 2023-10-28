// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClientUserWidget.generated.h"

UCLASS()
class WEEK9_API UClientUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMyNameText(FString _Name);

	void SetMyAngleText(double _Angle);

	void SetMySpeedText(float _Speed);

	void SetCurrentTurnText(int _Turn);

	void SetPlayer1Text(int _Player1Turn, FString _Player1Name);

	void SetPlayer2Text(int _Player2Turn, FString _Player2Name);

	void SetPlayer3Text(int _Player3Turn, FString _Player3Name);

	void SetPlayer4Text(int _Player4Turn, FString _Player4Name);
	
protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UTextBlock* MyName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UTextBlock* MyAngle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UTextBlock* MySpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UTextBlock* CurrentTurn;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UTextBlock* Player1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UTextBlock* Player2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UTextBlock* Player3;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UTextBlock* Player4;

};
