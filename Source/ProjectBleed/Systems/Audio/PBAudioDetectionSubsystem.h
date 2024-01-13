// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "../../PBProjectENUMS.h"
#include "PBAudioDetectionSubsystem.generated.h"

class UFMODAudioComponent;
class UFMODEvent;
class APBBackgroundMusicPlayer;

DECLARE_LOG_CATEGORY_EXTERN(LogPBAudioDetectionSubsystem, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBeatOccurSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHalfBeatOccurSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuarterBeatOccurSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGhostOnBeatOccurSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGhostOnHalfBeatOccurSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGhostOnQuarterBeatOccurSignature);
/**
 * 
 */

USTRUCT(BlueprintType)
struct FEventInformation
{
	GENERATED_BODY()
	
	UPROPERTY()
	int CurrentTempo = 0;

	UPROPERTY()
	int CurrentBeat = 0;

	UPROPERTY()
	int CurrentBar = 0;

	UPROPERTY()
	TMap<EBeatType, float> BeatIntervals = TMap<EBeatType, float>();

	UPROPERTY()
	bool bFinishSettingUp = false;

	bool IsFirstBeat() 
	{
		return CurrentBeat <= 1 && CurrentBar <= 1;
	}
};

UCLASS(Blueprintable)
class PROJECTBLEED_API UPBAudioDetectionSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnBeatOccurSignature OnBeatOccurDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnHalfBeatOccurSignature OnHalfBeatOccurDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnQuarterBeatOccurSignature OnQuarterBeatOccurDelegate;

	UPROPERTY(BlueprintAssignable)
	FGhostOnBeatOccurSignature OnGhostBeatOccurDelegate;

	UPROPERTY(BlueprintAssignable)
	FGhostOnHalfBeatOccurSignature OnGhostHalfBeatOccurDelegate;

	UPROPERTY(BlueprintAssignable)
	FGhostOnQuarterBeatOccurSignature OnGhostQuarterBeatOccurDelegate;
	
	UFUNCTION()
	void SetupSubsystem();
	
	//Loads, processes and plays audio
	UFUNCTION(BlueprintCallable, Category = "Audio", meta=(DefaultToSelf, CompactNodeTitle = "PB Play FMOD Audio",Keywords="Play, Process, Audio, Music"))
	void ProcessAndPlayAudio(UFMODEvent* EventToPlay);

	UFUNCTION(BlueprintCallable, Category = "Audio", meta = (DefaultToSelf, CompactNodeTitle = "PB Stop FMOD Audio", Keywords = "Stop, Music, Audio"))
	void StopAudio();

	UFUNCTION()
	void DelayedMusicPlay() const;

	UFUNCTION(BlueprintCallable, Category = "Audio", meta=(DefaultToSelf, CompactNodeTitle = "Get Event Information",Keywords="Event, Current, Audio, Music, Info, Information, Settings"))
	FEventInformation& GetCurrentEventInformation() { return CurrentEventInformation; }

	UFUNCTION(BlueprintCallable, Category = "Audio", meta=(DefaultToSelf, CompactNodeTitle = "Get Event Information",Keywords="Event, Current, Audio, Music, Info, Information, Settings"))
	FEventInformation& GetGhostEventInformation() { return GhostEventInformation; }

	UFUNCTION()
	void UpdateEventInformation(FEventInformation& EventInformation, int InBeat, int InBar);

	UFUNCTION(BlueprintCallable, Category = "Audio", BlueprintPure, meta=(Keywords ="Music, Player, Delay"))
	float GetMusicDelay();

	UFUNCTION(BlueprintCallable, Category = "Audio", BlueprintPure)
	float GetBeatLeniency() const { return BeatLeniency; }

	UFUNCTION(BlueprintCallable, Category= "Audio", BlueprintPure)
	float GetTimeSinceLastBeat(const EBeatType InBeatType = EBeatType::Any) const { return TimeSinceLastBeat[InBeatType]; }

	UFUNCTION()
	EBeatType GetLastPlayedBeat() const { return LastPlayedBeat; }

	/**
	 * Time till the next beat. Beat Interval - Time since last beat type.
	 * @param InBeatType Which next beat value do we want. If any, the next half beat is returned. Check WasOnBeat() comment.
	 */
	UFUNCTION(BlueprintCallable, Category = "Audio")
	float GetTimeTillNextBeat(const EBeatType InBeatType);
	
	UFUNCTION()
	FTimerHandle& GetDelayedMusicTimerHandle() { return DelayedMusicPlayTimerHandle; }

	/**
	* Checks if player was on beat by checking if the time since any last beat or time till next half beat is smaller than the Beat Leniency.
	* We use half step since that's the most accurate substep for the player since doing actions on quarter beat is not supported.
	* @param InBeatType What type of beat should we check against. Default is any ie. Whole and half.
	* @param OutAccuracy How accurate was the player in hitting the beat. 0 = 0%, 1 = 100%
	*/
	UFUNCTION()
	bool WasOnBeat(float& OutAccuracy, const EBeatType InBeatType = EBeatType::Half);

	UFUNCTION(BlueprintCallable, Category = "Audio")
	float GetBeatInterval(const EBeatType InBeatType = EBeatType::Half) const { return CurrentEventInformation.BeatIntervals[InBeatType]; }

	
protected:
	//Event information of the current event that's playing
	UPROPERTY()
	FEventInformation CurrentEventInformation;

	//Event information of the ghost event that's playing
	UPROPERTY()
	FEventInformation GhostEventInformation;

	//This is how much early or after a beat its considered to be "on beat".
	//Default to Quarter Beat Interval / 4
	UPROPERTY()
	float BeatLeniency = 0.1f;

	UPROPERTY()
	TMap<EBeatType, float> TimeSinceLastBeat;

	//Overridden Functions
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual TStatId GetStatId() const override;
	
	UFUNCTION()
	void OnBeatCallback(int32 Bar, int32 Beat, int32 Position, float Tempo, int32 TimeSignatureUpper, int32 TimeSignatureLower);

	UFUNCTION()
	void OnHalfBeatCallback();

	UFUNCTION()
	void OnQuarterBeatCallback();

	UFUNCTION()
	void OnGhostBeatCallback(int32 Bar, int32 Beat, int32 Position, float Tempo, int32 TimeSignatureUpper, int32 TimeSignatureLower);

	UFUNCTION()
	void OnGhostHalfBeatCallback();

	UFUNCTION()
	void OnGhostQuarterBeatCallback();

	UFUNCTION()
	void UpdateTimesSinceLastBeat(const float InDeltaTime);

	virtual void Tick(float DeltaTime) override;	

private:

	UPROPERTY()
	bool bRunMusicPlayErrorCorrection;

	UPROPERTY()
	FTimerHandle HalfBeatTimerHandle;

	UPROPERTY()
	FTimerHandle QuarterBeatTimerHandle;

	UPROPERTY()
	FTimerHandle GhostHalfBeatTimerHandle;

	UPROPERTY()
	FTimerHandle GhostQuarterBeatTimerHandle;

	UPROPERTY()
	FTimerHandle DelayedMusicPlayTimerHandle;

	UPROPERTY()
	UFMODEvent* CurrentEvent;

	//This will be referenced from APBBackgroundMusicPlayer, which is spawned during Initialize. Stored during Initialization of this subsystem
	UPROPERTY()
	UFMODAudioComponent*  BackgroundMusicComponent = nullptr;

	//This is also part of APBBackgroundMusicPlayer but this plays the song first, UI and anything else that needs to know what is coming can subscribe to this
	UPROPERTY()
	UFMODAudioComponent* GhostMusicComponent = nullptr;

	UPROPERTY()
	APBBackgroundMusicPlayer* BackgroundMusicPlayer;	

	UPROPERTY()
	EBeatType LastPlayedBeat;

	//Fills up CurrentEventInformation. Called from the first beat of an event
	UFUNCTION()
	void ProcessAudio(float Tempo);
};
