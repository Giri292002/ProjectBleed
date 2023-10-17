// All Rights belong to Studio WASD 2023

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PBInteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPBInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTBLEED_API IPBInteractableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void Interact(AActor* Interactor);

	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	int GetInteractionPriority();

	

};
