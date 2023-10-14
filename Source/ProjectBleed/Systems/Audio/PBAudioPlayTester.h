// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PBAudioPlayTester.generated.h"

class UFMODEvent;

UCLASS()
class PROJECTBLEED_API APBAudioPlayTester : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APBAudioPlayTester();

	UPROPERTY(EditAnywhere, Category = "Audio")
	UFMODEvent* MusicToPlay;

protected:
	UPROPERTY()
	FTimerHandle HalfBeatTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bPrintBeatDebug = false;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bPrintHalfBeatDebug = false;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bPrintQuarterBeatDebug = false;

protected:	
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnBeat();
	
	UFUNCTION()
	void OnHalfBeat();
	
	UFUNCTION()
	void OnQuarterBeat();
	
public:
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void PlayAudio();
};
