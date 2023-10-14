// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectBleed/Systems/Scoring/PBScoringSubsystem.h"
#include "PBDashComponent.generated.h"

class UCharacterMovementComponent;
class APBCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTBLEED_API UPBDashComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPBDashComponent();

	UFUNCTION()
	void PerformDash(const FVector2D& DashDirection);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Locomotion|Dash")
	float MovementMultiplier;

	UPROPERTY(EditDefaultsOnly, Category = "Locomotion|Dash")
	UPBScoreData* ScoreData;
	
	UPROPERTY()
	APBCharacter* OwnerCharacter;

	UFUNCTION()
	void BeginPlay() override;

private:
	UPROPERTY()
	float DashVelocity;
};