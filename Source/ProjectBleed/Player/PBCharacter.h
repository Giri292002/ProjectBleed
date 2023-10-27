// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectBleed/Input/PBInput.h"
#include "PBCharacter.generated.h"

struct FInputActionValue;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
class UPBInteractionComponent;
class UPBDashComponent;
class UPBCombatComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogPBCharacter, Log, All);

UCLASS()
class PROJECTBLEED_API APBCharacter : public ACharacter, public IPBInput
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APBCharacter();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "ProjectBleed | Character | Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, Category = "ProjectBleed | Character | Camera")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere, Category="ProjectBleed | Character | Camera")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, Category = "ProjectBleed | Character | Locomotion")
	UPBDashComponent* DashComponent;

	UPROPERTY(EditAnywhere, Category = "ProjectBleed | Character | Interaction")
	UPBInteractionComponent* InteractionComponent;

	UPROPERTY(EditAnywhere, Category = "ProjectBleed | Character | Combat")
	UPBCombatComponent* CombatComponent;

	UPROPERTY()
	FVector2D MovementDirection;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Input Interface Implementation
	virtual UInputMappingContext* GetInputMappingContext_Implementation() override;

	UFUNCTION()
	FVector GetCharacterDirection() const;

	UFUNCTION()
	void Move(const FInputActionValue& ActionValue);

	UFUNCTION()
	void Dash();

	UFUNCTION()
	void Interact();

	UFUNCTION()
	void StartFire();

	UFUNCTION()
	void StopFire();
};
