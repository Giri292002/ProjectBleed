// Fill out your copyright notice in the Description page of Project Settings.


#include "PBAudioDetectionSubsystem.h"

#include "FMODAudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PBBackgroundMusicPlayer.h"
#include "ProjectBleed/Libraries/CustomLogging.h"
#include "ProjectBleed/Settings/PBAudioDetectionSettings.h"

DEFINE_LOG_CATEGORY(LogPBAudioDetectionSubsystem)

void UPBAudioDetectionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
#if !WITH_EDITOR
	SetupSubsystem();
#endif
	const UPBAudioDetectionSettings* Settings = GetDefault<UPBAudioDetectionSettings>();
	if (IsValid(Settings))
	{
		BeatLeniencyCorrection = Settings->BeatLeniencyCorrection;
		AccuracyCutoff = Settings->AccuracyCutoff;
	}
}

void UPBAudioDetectionSubsystem::SetupSubsystem()
{
	TimeSincePreviousBeat.Add(EBeatType::Whole, 0.f);
	TimeSincePreviousBeat.Add(EBeatType::Half, 0.f);
	TimeSincePreviousBeat.Add(EBeatType::Quarter, 0.f);
	TimeSincePreviousBeat.Add(EBeatType::Any, 0.f);

	//Dont want to create double actors
	if (IsValid(BackgroundMusicPlayer))
		return;
	BackgroundMusicPlayer = Cast<APBBackgroundMusicPlayer>(GetWorld()->SpawnActor(APBBackgroundMusicPlayer::StaticClass()));
	if (IsValid(BackgroundMusicPlayer))
	{
		BackgroundMusicComponent = BackgroundMusicPlayer->GetBackgroundMusicComponent();
		BackgroundMusicComponent->OnTimelineBeat.AddDynamic(this, &UPBAudioDetectionSubsystem::OnBeatCallback);
		GhostMusicComponent = BackgroundMusicPlayer->GetGhostBackgroundMusicComponent();
		GhostMusicComponent->OnTimelineBeat.AddDynamic(this, &UPBAudioDetectionSubsystem::OnGhostBeatCallback);
		bRunMusicPlayErrorCorrection = false;
	}
}

void UPBAudioDetectionSubsystem::ProcessAndPlayAudio(UFMODEvent* EventToPlay)
{
	if(!IsValid(BackgroundMusicComponent))
	{
		SetupSubsystem();
	}
	if(EventToPlay != nullptr)
	{
		CurrentEvent = EventToPlay;
		//The first beat of this event will process the audio.
		GhostMusicComponent->SetEvent(EventToPlay);
		GhostMusicComponent->SetParameter("BGMVolume", 0.f);
		bRunMusicPlayErrorCorrection = true;
		GhostMusicComponent->Play();
	}
}

void UPBAudioDetectionSubsystem::StopAudio()
{
	if (IsValid(BackgroundMusicComponent))
	{
		//Check if component is currently playing
		if (BackgroundMusicComponent->IsPlaying())
		{
			BackgroundMusicComponent->Stop();
			GetGhostEventInformation() = FEventInformation();
			GetCurrentEventInformation() = FEventInformation();
		}
	}
}

void UPBAudioDetectionSubsystem::DelayedMusicPlay() const
{
	BackgroundMusicComponent->SetEvent(CurrentEvent);
	BackgroundMusicComponent->SetParameter("BGMVolume", 1.f);
	BackgroundMusicComponent->Play();
}

void UPBAudioDetectionSubsystem::UpdateEventInformation(FEventInformation& EventInformation, int InBeat, int InBar)
{
	EventInformation.CurrentBeat = InBeat;
	EventInformation.CurrentBar = InBar;
}

float UPBAudioDetectionSubsystem::GetMusicDelay()
{
	if(IsValid(BackgroundMusicPlayer))
	{
		return BackgroundMusicPlayer->GetMusicDelay();
	}
	return -1.f;
}

float UPBAudioDetectionSubsystem::GetTimeTillNextBeat(const EBeatType InBeatType)
{
	if(InBeatType == EBeatType::Any)
	{
		return GetCurrentEventInformation().BeatIntervals[GLOBAL_BEAT_STEP_ACCURACY] - GetTimeSinceLastBeat(GLOBAL_BEAT_STEP_ACCURACY);
	}
	return GetCurrentEventInformation().BeatIntervals[InBeatType] - GetTimeSinceLastBeat(InBeatType);	
}

float UPBAudioDetectionSubsystem::GetTimeSincePreviousBeat(const EBeatType InBeatType)
{
	return TimeSincePreviousBeat[InBeatType];
}

bool UPBAudioDetectionSubsystem::WasOnBeat( double& OutAccuracy, const EBeatType InBeatType) const
{
	if (!IsMusicPlaying())
	{
		return false;
	}

	const double ActionPerformedTime = GetWorld()->GetRealTimeSeconds();

	double RegisteredTimeOfPreviousBeat;
	if (!GetTimeOfPreviousBeat(GLOBAL_BEAT_STEP_ACCURACY, RegisteredTimeOfPreviousBeat))
	{
		return false;
	}

	double RegisteredTimeOfNextBeat;
	if (!GetTimeOfNextBeat(GLOBAL_BEAT_STEP_ACCURACY, RegisteredTimeOfNextBeat))
	{
		return false;
	}

	//Just using this variable here to know what the beat leniency is during breakpoint debugging
	const float TestBeatLeniency = GetBeatLeniency();

	//Accuracy is measured from 0 to 1;
	double AccuracySinceLastBeat = 0.f;
	double AccuracyToNextBeat = 0.f;

	FString LogText = "ON BEAT DAMN!";
	
	//Since the action was performed under the leniency time this considered being fully accurate
	if (ActionPerformedTime < RegisteredTimeOfPreviousBeat + GetBeatLeniency())
	{
		AccuracySinceLastBeat = 1.f;
	}
	else if (ActionPerformedTime > RegisteredTimeOfPreviousBeat + GetBeatLeniency())
	//They might be waayy off beat since they are greater their action happened after the lenient time we have set. Calculate just how far off they are.
	{ 
		AccuracySinceLastBeat = UKismetMathLibrary::MapRangeUnclamped(ActionPerformedTime, RegisteredTimeOfPreviousBeat + GetBeatLeniency(), RegisteredTimeOfPreviousBeat, 0.f, 1.f);
	}

	//They might have anticipated the next beat and performed the action if they are still in the leniency window, consider this as fully accurate
	if (ActionPerformedTime > RegisteredTimeOfNextBeat - GetBeatLeniency())
	{
		AccuracyToNextBeat = 1.f;
	}
	//Yet again they may be wayy off since they performed the action way earlier than our lenient time window. Calculate just how far off are they.
	else if	(ActionPerformedTime < RegisteredTimeOfNextBeat - GetBeatLeniency())
	{
		AccuracyToNextBeat = UKismetMathLibrary::MapRangeUnclamped(ActionPerformedTime, RegisteredTimeOfNextBeat - GetBeatLeniency(), RegisteredTimeOfNextBeat, 0.f, 1.f);
	}

	//Choose the highest accuracy we have
	OutAccuracy = FMath::Max(AccuracySinceLastBeat, AccuracyToNextBeat);

	//Purely for debug purposes
	const double OutAccuracyBeforeCutoff = OutAccuracy;

	//We can't be registering values less than AccuracyCutoff as being on beat so cut it off
	OutAccuracy = OutAccuracy < AccuracyCutoff ? 0.f : OutAccuracy;

	LogText = FString::Printf(TEXT("ACCURACY IS %f"), OutAccuracy);
	LogText = AccuracySinceLastBeat > AccuracyToNextBeat ? LogText.Append(" USING AFTER BEAT ACCURACY. YOU JUST MISSED THE BEAT.") : LogText.Append(" USING BEFORE BEAT. YOU ARE ANTICIPATING THE BEAT");

	if (OutAccuracy < AccuracyCutoff)
	{
		LogText = FString::Printf(TEXT("TOTALLY OFF BEAT.ACCURACY IS %f"), OutAccuracyBeforeCutoff);
	}

	V_LOG(LogPBAudioDetectionSubsystem, LogText);
	return OutAccuracy >= AccuracyCutoff;
}

bool UPBAudioDetectionSubsystem::GetTimeOfPreviousBeat(EBeatType const InBeatType, double& OutTimeOfPreviousBeat) const
{
	if (!TimeOfPreviousBeat.Find(InBeatType))
	{
		return false;
	}
	OutTimeOfPreviousBeat = TimeOfPreviousBeat[InBeatType];
	return true;
}

bool UPBAudioDetectionSubsystem::GetTimeOfNextBeat(EBeatType const InBeatType, double& OutTimeOfNextBeat) const
{
	double OutTimeOfPreviousBeat;
	if (!GetTimeOfPreviousBeat(InBeatType, OutTimeOfPreviousBeat))
	{
		return false;
	}
	OutTimeOfNextBeat = OutTimeOfPreviousBeat + GetBeatInterval(InBeatType);
	return true;
}

bool UPBAudioDetectionSubsystem::IsMusicPlaying() const
{
	if (!ensureAlwaysMsgf(BackgroundMusicComponent, TEXT("Background Music is playing")))
	{
		return false;
	}

	return BackgroundMusicComponent->IsPlaying();
}

void UPBAudioDetectionSubsystem::ProcessAudio(float InTempo)
{
	GetGhostEventInformation().CurrentTempo = InTempo;
	GetGhostEventInformation().BeatIntervals.Add(EBeatType::Whole, 60.f / InTempo);
	GetGhostEventInformation().BeatIntervals.Add(EBeatType::Half, GetGhostEventInformation().BeatIntervals[EBeatType::Whole] / 2.f);
	GetGhostEventInformation().BeatIntervals.Add(EBeatType::Quarter, GetGhostEventInformation().BeatIntervals[EBeatType::Half] / 2.f);
	GetGhostEventInformation().bFinishSettingUp = true;

	BeatLeniency = (GetGhostEventInformation().BeatIntervals[EBeatType::Quarter] / 2.f) + BeatLeniencyCorrection;
	
	CurrentEventInformation = GhostEventInformation;

	UE_LOG(LogPBAudioDetectionSubsystem, Log,
	TEXT("Finished processing Audio.\n Tempo: %f \n Whole Beat Interval: %f \n Half Beat Interval: %f \n Quarter Beat Interval: %f \n Beat Leniency: %f "),
	GetCurrentEventInformation().CurrentTempo,
	GetCurrentEventInformation().BeatIntervals[EBeatType::Whole],
	GetCurrentEventInformation().BeatIntervals[EBeatType::Half],
	GetCurrentEventInformation().BeatIntervals[EBeatType::Quarter],
	GetBeatLeniency());
}

bool UPBAudioDetectionSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	//No need to call super since we are copy pasting the same code but
	//just making sure subsystem doesnt get spawned in editor
	if (!Super::ShouldCreateSubsystem(Outer))
	{
		return false;
	}

	const UWorld* World = Cast<UWorld>(Outer);
	check(World);
	return World->WorldType == EWorldType::Game  || World->WorldType == EWorldType::PIE;	
}

TStatId UPBAudioDetectionSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UPBAudioDetectionSubsystem, STATGROUP_Tickables);
}

void UPBAudioDetectionSubsystem::OnBeatCallback(int32 Bar, int32 Beat, int32 Position, float Tempo,
                                                int32 TimeSignatureUpper, int32 TimeSignatureLower)
{	
	UpdateEventInformation(GetCurrentEventInformation(), Bar, Beat);

	TimeSincePreviousBeat[EBeatType::Any] = 0.f;
	TimeSincePreviousBeat[EBeatType::Whole] = 0.f;
	TimeSincePreviousBeat[EBeatType::Half] = 0.f;
	TimeSincePreviousBeat[EBeatType::Quarter] = 0.f;
	
	RecordTimeOfBeat(EBeatType::Whole);
	OnBeatOccurDelegate.Broadcast();
	
	if(GetCurrentEventInformation().bFinishSettingUp)
	{
		GetWorld()->GetTimerManager().SetTimer(HalfBeatTimerHandle, this, &UPBAudioDetectionSubsystem::OnHalfBeatCallback, GetCurrentEventInformation().BeatIntervals[EBeatType::Half]);
		GetWorld()->GetTimerManager().SetTimer(QuarterBeatTimerHandle, this, &UPBAudioDetectionSubsystem::OnQuarterBeatCallback, GetCurrentEventInformation().BeatIntervals[EBeatType::Quarter]);
	}
}

void UPBAudioDetectionSubsystem::OnHalfBeatCallback()
{
	RecordTimeOfBeat(EBeatType::Half);
	OnHalfBeatOccurDelegate.Broadcast();
	TimeSincePreviousBeat[EBeatType::Any] = 0.f;
	TimeSincePreviousBeat[EBeatType::Half] = 0.f;
	TimeSincePreviousBeat[EBeatType::Quarter] = 0.f;
	GetWorld()->GetTimerManager().SetTimer(HalfBeatTimerHandle, this, &UPBAudioDetectionSubsystem::OnHalfBeatCallback, GetCurrentEventInformation().BeatIntervals[EBeatType::Half]);
}

void UPBAudioDetectionSubsystem::OnQuarterBeatCallback()
{
	RecordTimeOfBeat(EBeatType::Quarter);
	OnQuarterBeatOccurDelegate.Broadcast();
	TimeSincePreviousBeat[EBeatType::Any] = 0.f;
	TimeSincePreviousBeat[EBeatType::Quarter] = 0.f;
	GetWorld()->GetTimerManager().SetTimer(QuarterBeatTimerHandle, this, &UPBAudioDetectionSubsystem::OnQuarterBeatCallback, GetCurrentEventInformation().BeatIntervals[EBeatType::Quarter]);
}

void UPBAudioDetectionSubsystem::OnGhostBeatCallback(int32 Bar, int32 Beat, int32 Position, float Tempo,
	int32 TimeSignatureUpper, int32 TimeSignatureLower)
{
	bRunMusicPlayErrorCorrection = false;
	UpdateEventInformation(GetGhostEventInformation(), Bar, Beat);
	if(GetGhostEventInformation().IsFirstBeat())
	{
		GetGhostEventInformation().bFinishSettingUp = false;
		GetWorld()->GetTimerManager().SetTimer(DelayedMusicPlayTimerHandle, this, &UPBAudioDetectionSubsystem::DelayedMusicPlay, BackgroundMusicPlayer->GetMusicDelay());
		ProcessAudio(Tempo);
	}

	OnGhostBeatOccurDelegate.Broadcast();

	if(GetGhostEventInformation().bFinishSettingUp)
	{
		GetWorld()->GetTimerManager().SetTimer(GhostHalfBeatTimerHandle, this, &UPBAudioDetectionSubsystem::OnGhostHalfBeatCallback, GetCurrentEventInformation().BeatIntervals[EBeatType::Half]);
		GetWorld()->GetTimerManager().SetTimer(GhostQuarterBeatTimerHandle, this, &UPBAudioDetectionSubsystem::OnGhostQuarterBeatCallback, GetCurrentEventInformation().BeatIntervals[EBeatType::Quarter]);
	}
}

void UPBAudioDetectionSubsystem::OnGhostHalfBeatCallback()
{
	OnGhostHalfBeatOccurDelegate.Broadcast();
	GetWorld()->GetTimerManager().SetTimer(GhostHalfBeatTimerHandle, this, &UPBAudioDetectionSubsystem::OnGhostHalfBeatCallback, GetCurrentEventInformation().BeatIntervals[EBeatType::Half]);
}

void UPBAudioDetectionSubsystem::OnGhostQuarterBeatCallback()
{
	OnGhostQuarterBeatOccurDelegate.Broadcast();
	GetWorld()->GetTimerManager().SetTimer(GhostQuarterBeatTimerHandle, this, &UPBAudioDetectionSubsystem::OnGhostHalfBeatCallback, GetCurrentEventInformation().BeatIntervals[EBeatType::Quarter]);
}

void UPBAudioDetectionSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateTimesSinceLastBeat(DeltaTime);
}

void UPBAudioDetectionSubsystem::UpdateTimesSinceLastBeat(const float InDeltaTime)
{
	for (TPair<EBeatType, float>& Pair : TimeSincePreviousBeat)
	{
		Pair.Value += InDeltaTime;
	}
}

void UPBAudioDetectionSubsystem::RecordTimeOfBeat(EBeatType const InBeatType)
{
	if (!ensureAlwaysMsgf(InBeatType != EBeatType::Any, TEXT("Beat type cannot be any beat type. Please specify a particular beat type")))
	{
		return;
	}
	TimeOfPreviousBeat.Add(InBeatType, GetWorld()->GetRealTimeSeconds());
}
