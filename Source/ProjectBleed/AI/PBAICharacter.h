// All Rights belong to Studio WASD 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PBAICharacter.generated.h"

class UPBHealthComponent;

UCLASS()
class PROJECTBLEED_API APBAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APBAICharacter();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "ProjectBleed | Health")
	UPBHealthComponent* HealthComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
