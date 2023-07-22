// Copyright Epic Games, Inc. All Rights Reserved.

#include "SwashGameMode.h"
#include "SwashCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASwashGameMode::ASwashGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
