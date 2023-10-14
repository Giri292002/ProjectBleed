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

class UPBDashComponent;
UCLASS()
class PROJECTBLEED_API APBCharacter : public ACharacter, public IPBInput
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APBCharacter();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, Category = "Camera")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere, Category="Camera")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, Category = "Locomotion")
	UPBDashComponent* DashComponent;

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
};
