// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectBleedGameMode.h"

#include "PBGameState.h"
#include "../Core/PBHUD.h"
#include "../Player/PBCharacter.h"
#include "../Player/PBPlayerController.h"

AProjectBleedGameMode::AProjectBleedGameMode()
{
	DefaultPawnClass = APBCharacter::StaticClass();
	PlayerControllerClass = APBPlayerController::StaticClass();
	HUDClass = APBHUD::StaticClass();
	GameStateClass = APBGameState::StaticClass();
}
