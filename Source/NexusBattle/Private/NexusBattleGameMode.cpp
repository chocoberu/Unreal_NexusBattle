// Copyright Epic Games, Inc. All Rights Reserved.

#include "NexusBattleGameMode.h"
#include "NexusBattleCharacter.h"
#include "UObject/ConstructorHelpers.h"

ANexusBattleGameMode::ANexusBattleGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
