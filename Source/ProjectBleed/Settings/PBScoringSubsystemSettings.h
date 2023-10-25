// All Rights belong to Studio WASD 2023

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "../../../Plugins/FMODStudio/Source/FMODStudio/Classes/FMODEvent.h"
#include "PBScoringSubsystemSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, defaultconfig, meta=(DisplayName="Scoring Subsystem Settings"))
class PROJECTBLEED_API UPBScoringSubsystemSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
	public:
		UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Scoring")
		TSoftObjectPtr<UFMODEvent> OnAccurateHitEventPath;
};
