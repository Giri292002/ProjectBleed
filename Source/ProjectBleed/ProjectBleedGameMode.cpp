// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectBleedGameMode.h"
#include "ProjectBleedCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectBleedGameMode::AProjectBleedGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
