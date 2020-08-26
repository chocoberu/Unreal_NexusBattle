// Fill out your copyright notice in the Description page of Project Settings.


#include "NBPlayerController.h"

void ANBPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	NBLOG_S(Warning);
}
void ANBPlayerController::OnPossess(APawn* aPawn)
{
	NBLOG_S(Warning);
	Super::OnPossess(aPawn);
}
