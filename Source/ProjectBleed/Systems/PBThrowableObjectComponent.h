// All Rights belong to Studio WASD 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PBThrowableObjectComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPBThrowableComponent, Log, All)

//Attach this component to any actor that needs to be thrown
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTBLEED_API UPBThrowableObjectComponent : public UProjectileMovementComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPBThrowableObjectComponent();

	//Throws the object this component is attached to
	UFUNCTION(BlueprintCallable)
	void Throw();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
