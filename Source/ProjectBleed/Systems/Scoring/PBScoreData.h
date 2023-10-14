// All Rights belong to Studio WASD 2023

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PBScoreData.generated.h"

UCLASS()
class PROJECTBLEED_API UPBScoreData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Score", meta=(ClampMin = 0))
	int ScoreToAdd = 10; 
};
