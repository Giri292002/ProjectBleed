// All Rights belong to Studio WASD 2023

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PBInteractionComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPBInteractionComponent, Log, All);

class APBCharacter;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTBLEED_API UPBInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPBInteractionComponent();

protected:
	UPROPERTY()
	APBCharacter* PBOwnerCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction | Trace")
	float TraceRadius = 50.f;

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	//Interact with the object with highest priority
	void Interact();
};
