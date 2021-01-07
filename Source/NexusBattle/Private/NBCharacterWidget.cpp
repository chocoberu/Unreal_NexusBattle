// Fill out your copyright notice in the Description page of Project Settings.


#include "NBCharacterWidget.h"
#include "NBCharacterStatComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UNBCharacterWidget::BindCharacterStat(UNBCharacterStatComponent* NewCharacterStat)
{
	NBCHECK(NewCharacterStat != nullptr);

	CurrentCharacterStat = NewCharacterStat;
	NewCharacterStat->OnHPChanged.AddUObject(this, &UNBCharacterWidget::UpdateWidget);
	NewCharacterStat->OnLevelChanged.AddUObject(this, &UNBCharacterWidget::UpdateWidget);
}

void UNBCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	NBCHECK(HPProgressBar != nullptr);
	MPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_MPBar")));
	NBCHECK(MPProgressBar != nullptr);
	LevelText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TEXT_Level")));
	NBCHECK(LevelText != nullptr);
	UpdateWidget();
}

void UNBCharacterWidget::UpdateWidget()
{
	if (HPProgressBar != nullptr)
		HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
	if (MPProgressBar != nullptr)
		MPProgressBar->SetPercent(CurrentCharacterStat->GetMPRatio());
	if (LevelText != nullptr)
		LevelText->SetText(FText::FromString(FString::FromInt(CurrentCharacterStat->GetLevel())));
	
}
