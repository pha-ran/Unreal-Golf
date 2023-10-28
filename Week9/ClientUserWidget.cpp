// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientUserWidget.h"
#include "Components/TextBlock.h"

void UClientUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	MyName = Cast<UTextBlock>(GetWidgetFromName(TEXT("NameText")));

	MyAngle = Cast<UTextBlock>(GetWidgetFromName(TEXT("AngleText")));

	MySpeed = Cast<UTextBlock>(GetWidgetFromName(TEXT("SpeedText")));

	CurrentTurn = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrentTurnText")));

	Player1 = Cast<UTextBlock>(GetWidgetFromName(TEXT("Player1Text")));

	Player2 = Cast<UTextBlock>(GetWidgetFromName(TEXT("Player2Text")));

	Player3 = Cast<UTextBlock>(GetWidgetFromName(TEXT("Player3Text")));

	Player4 = Cast<UTextBlock>(GetWidgetFromName(TEXT("Player4Text")));
}

void UClientUserWidget::SetMyNameText(FString _Name)
{
	if (MyName != nullptr)
	{
		FString FS = "" + _Name;

		MyName->SetText(FText::FromString(FS));
	}
}

void UClientUserWidget::SetMyAngleText(double _Angle)
{
	if (MyAngle != nullptr)
	{
		FString FS = FString::Printf(TEXT("각도 : %f"), _Angle);

		MyAngle->SetText(FText::FromString(FS));
	}
}

void UClientUserWidget::SetMySpeedText(float _Speed)
{
	if (MySpeed != nullptr)
	{
		FString FS = FString::Printf(TEXT("속도 : %f"), _Speed);

		MySpeed->SetText(FText::FromString(FS));
	}
}

void UClientUserWidget::SetCurrentTurnText(int _Turn)
{
	if (CurrentTurn != nullptr)
	{
		FString FS = FString::Printf(TEXT("현재 턴 : %d"), _Turn);

		CurrentTurn->SetText(FText::FromString(FS));
	}
}

void UClientUserWidget::SetPlayer1Text(int _Player1Turn, FString _Player1Name)
{
	if (Player1 != nullptr)
	{
		FString FS = FString::Printf(TEXT("%d"), _Player1Turn) + _Player1Name;

		Player1->SetText(FText::FromString(FS));
	}
}

void UClientUserWidget::SetPlayer2Text(int _Player2Turn, FString _Player2Name)
{
	if (Player2 != nullptr)
	{
		FString FS = FString::Printf(TEXT("%d"), _Player2Turn) + _Player2Name;

		Player2->SetText(FText::FromString(FS));
	}
}

void UClientUserWidget::SetPlayer3Text(int _Player3Turn, FString _Player3Name)
{
	if (Player3 != nullptr)
	{
		FString FS = FString::Printf(TEXT("%d"), _Player3Turn) + _Player3Name;

		Player3->SetText(FText::FromString(FS));
	}
}

void UClientUserWidget::SetPlayer4Text(int _Player4Turn, FString _Player4Name)
{
	if (Player4 != nullptr)
	{
		FString FS = FString::Printf(TEXT("%d"), _Player4Turn) + _Player4Name;

		Player4->SetText(FText::FromString(FS));
	}
}
