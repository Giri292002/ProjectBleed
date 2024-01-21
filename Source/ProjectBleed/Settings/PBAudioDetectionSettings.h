// All Rights belong to Studio WASD 2023

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "PBAudioDetectionSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Audio Detection Subsystem Settings"))
class PROJECTBLEED_API UPBAudioDetectionSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	//This is how much early or after a beat its considered to be "on beat".
	//Default is Quarter Beat Interval / 4 + BeatLeniencyCorrection
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Scoring")
	float BeatLeniencyCorrection = 0.1;
	
};
