// All Rights belong to Studio WASD 2023

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PBHealthData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBLEED_API UPBHealthData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "ProjectBleed | Default")
	int MaxHealth;
	
};
