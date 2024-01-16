// All Rights belong to Studio WASD 2023

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PBAIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBLEED_API APBAIControllerBase : public AAIController
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	UBlackboardComponent* BlackboardComponent;	
};
