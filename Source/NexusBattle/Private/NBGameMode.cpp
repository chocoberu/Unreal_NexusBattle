// Fill out your copyright notice in the Description page of Project Settings.

#include "NBGameMode.h"
#include "NBPlayerController.h"

ANBGameMode::ANBGameMode()
{
	PlayerControllerClass = ANBPlayerController::StaticClass();
}

void ANBGameMode::PostLogin(APlayerController* NewPlayer)
{
	NBLOG(Warning, TEXT("PostLogin Begin"));
	Super::PostLogin(NewPlayer);
	NBLOG(Warning, TEXT("PostLogin End"));
}
