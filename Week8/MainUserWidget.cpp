// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUserWidget.h"
#include "Components/TextBlock.h"

void UMainUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerTurnText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TurnText")));

	PlayerNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("NameText")));

	PlayerUniqueIdText = Cast<UTextBlock>(GetWidgetFromName(TEXT("UniqueIdText")));
}

void UMainUserWidget::SetPlayerText(FString Turn, FString Name, FString UniqueId)
{
	if (PlayerTurnText != nullptr && PlayerNameText != nullptr && PlayerUniqueIdText != nullptr)
	{
		PlayerTurnText->SetText(FText::FromString(Turn));
		PlayerNameText->SetText(FText::FromString(Name));
		PlayerUniqueIdText->SetText(FText::FromString(UniqueId));
	}
}
