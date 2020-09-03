// Fill out your copyright notice in the Description page of Project Settings.


#include "NBMinionWidget.h"
#include "Components/ProgressBar.h"
#include "NBNormalMinion.h"

void UNBMinionWidget::BindMinion(ANBNormalMinion* NewMinion)
{
	NBCHECK(NewMinion != nullptr);

	Minion = NewMinion;
	NewMinion->OnHPChanged.AddUObject(this, &UNBMinionWidget::UpdateWidget);
}
void UNBMinionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	NBCHECK(HPProgressBar != nullptr);
	UpdateWidget();
}

void UNBMinionWidget::UpdateWidget()
{
	if (HPProgressBar != nullptr)
	{
		HPProgressBar->SetPercent(Minion->GetHPRatio());
	}
}
