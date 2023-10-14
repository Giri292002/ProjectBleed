// All Rights belong to Studio WASD 2023

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PBScoreData.h"
#include "PBScoringSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPBScoringSubsystem, Log, All);

UCLASS()
class PROJECTBLEED_API UPBScoringSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "PB Scoring System")
	int GetCurrentScore() const { return CurrentScore; }

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
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual TStatId GetStatId() const override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	UFUNCTION()
	void SetupSubsystem();
};
