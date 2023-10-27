// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PBPlayerController.generated.h"

/**
 * 
 */
class UInputAction;
UCLASS()
class PROJECTBLEED_API APBPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	//Returns normalized mouse world location
	UFUNCTION()
	FVector GetMouseWorldLocation();

	//Returns the direction from the player to the mouse
	UFUNCTION()
	FVector GetMouseWorldDirection(const FVector& InWorldLocation);

protected:
	UPROPERTY(EditDefaultsOnly)
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* DashAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* InteractAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* StartFireAction;
	
	UPROPERTY(EditDefaultsOnly)
	UInputAction* StopFireAction;

	UPROPERTY()
	FRotator NewControlRotation = FRotator::ZeroRotator;

protected:
	UFUNCTION()
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	virtual void OnUnPossess() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void UpdateNewControlRotationToCursor();

	UFUNCTION()
	void UpdateControlRotation();
};
