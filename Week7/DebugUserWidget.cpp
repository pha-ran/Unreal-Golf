// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugUserWidget.h"
#include "Components/TextBlock.h"

void UDebugUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerCodeText = Cast<UTextBlock>(GetWidgetFromName(TEXT("PlayerCodeText")));
}

void UDebugUserWidget::SetPlayerCodeText(FString code)
{
	PlayerCodeText->SetText(FText::FromString(code));
}
