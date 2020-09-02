// Fill out your copyright notice in the Description page of Project Settings.


#include "NBCharacterWidget.h"
#include "NBCharacterStatComponent.h"
#include "Components/ProgressBar.h"

void UNBCharacterWidget::BindCharacterStat(UNBCharacterStatComponent* NewCharacterStat)
{
	NBCHECK(NewCharacterStat != nullptr);

	CurrentCharacterStat = NewCharacterStat;
	NewCharacterStat->OnHPChanged.AddUObject(this, &UNBCharacterWidget::UpdateWidget);
}

void UNBCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	NBCHECK(HPProgressBar != nullptr);
	UpdateWidget();
}

void UNBCharacterWidget::UpdateWidget()
{
	if (HPProgressBar != nullptr)
		HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
	
}
