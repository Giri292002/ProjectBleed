// All Rights belong to Studio WASD 2023

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PBScoreData.h"
#include "PBScoringSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPBScoringSubsystem, Log, All);

class UFMODEvent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreUpdateSignature, int, NewScore);

UCLASS()
class PROJECTBLEED_API UPBScoringSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "PB Scoring System")
	FOnScoreUpdateSignature OnScoreUpdate;

	//Returns the score with multiploer
	UFUNCTION(BlueprintCallable, Category = "PB Scoring System")
	int GetCurrentScore() const { return CurrentScore; }

	// Returns the normal score without multiplier
	UFUNCTION(BlueprintCallable, Category = "PB Scoring System")
	int GetBaseScore() const { return BaseScore; }

	UFUNCTION(BlueprintCallable, Category = "PB Scoring System")
	int GetScoreMultiplier() const { return CurrentScoreMultiplier; }

	UFUNCTION(BlueprintCallable, Category = "PB Scoring System")
	void AddToScore(const UPBScoreData* InScore);

protected:
	//The final score with multiplier
	UPROPERTY()
	int CurrentScore = 0;

	//Normal score without multiplier
	UPROPERTY()
	int BaseScore = 0;

	UPROPERTY()
	int CurrentScoreMultiplier = 1;

	//How much score should we reduce every beat. Set from Scoring Settings
	UPROPERTY()
	int TrickleDownAmount = 1;

	UPROPERTY()
	UFMODEvent* OnAccurateHitEvent;

	UFUNCTION()
	void OnBeatOccur();

	/*
	* Reduces score based on a delta
	* @param InScoreToReduce The Amount of score to reduce
	*/
	UFUNCTION()
	void ReduceScore(const int InScoreToReduce);

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual TStatId GetStatId() const override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
};
