// Fill out your copyright notice in the Description page of Project Settings.


#include "PBAudioDetectionSubsystem.h"

#include "FMODAudioComponent.h"
#include "PBBackgroundMusicPlayer.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogPBAudioDetectionSubsystem)

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
		return GetCurrentEventInformation().BeatIntervals[EBeatType::Half] - GetTimeSinceLastBeat(EBeatType::Half);
	}
	return GetCurrentEventInformation().BeatIntervals[InBeatType] - GetTimeSinceLastBeat(InBeatType);	
}

void UPBAudioDetectionSubsystem::SetupSubsystem()
{
	TimeSinceLastBeat.Add(EBeatType::Whole, 0.f);
	TimeSinceLastBeat.Add(EBeatType::Half, 0.f);
	TimeSinceLastBeat.Add(EBeatType::Quarter, 0.f);
	TimeSinceLastBeat.Add(EBeatType::Any, 0.f);
	
	//Dont want to create double actors
	if(IsValid(BackgroundMusicPlayer))
		return;
	BackgroundMusicPlayer = Cast<APBBackgroundMusicPlayer>(GetWorld()->SpawnActor(APBBackgroundMusicPlayer::StaticClass()));
	if(IsValid(BackgroundMusicPlayer))
	{
		BackgroundMusicComponent = BackgroundMusicPlayer->GetBackgroundMusicComponent();
		BackgroundMusicComponent->OnTimelineBeat.AddDynamic(this, &UPBAudioDetectionSubsystem::OnBeatCallback);
		GhostMusicComponent = BackgroundMusicPlayer->GetGhostBackgroundMusicComponent();
		GhostMusicComponent->OnTimelineBeat.AddDynamic(this, &UPBAudioDetectionSubsystem::OnGhostBeatCallback);
		bRunMusicPlayErrorCorrection = false;
	}
}

bool UPBAudioDetectionSubsystem::WasOnBeat( float& OutAccuracy, const EBeatType InBeatType)
{
	return TimeSinceLastBeat[EBeatType::Half] <= GetBeatLeniency() || GetTimeTillNextBeat(InBeatType) < GetBeatLeniency();
}

void UPBAudioDetectionSubsystem::ProcessAudio(float InTempo)
{
	GetGhostEventInformation().CurrentTempo = InTempo;
	GetGhostEventInformation().BeatIntervals.Add(EBeatType::Whole, 60.f / InTempo);
	GetGhostEventInformation().BeatIntervals.Add(EBeatType::Half, GetGhostEventInformation().BeatIntervals[EBeatType::Whole] / 2.f);
	GetGhostEventInformation().BeatIntervals.Add(EBeatType::Quarter, GetGhostEventInformation().BeatIntervals[EBeatType::Half] / 2.f);
	GetGhostEventInformation().bFinishSettingUp = true;

	BeatLeniency = GetGhostEventInformation().BeatIntervals[EBeatType::Quarter] / 4.f;
	
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

void UPBAudioDetectionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
#if !WITH_EDITOR
	SetupSubsystem();
#endif
}

TStatId UPBAudioDetectionSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UPBAudioDetectionSubsystem, STATGROUP_Tickables);
}

void UPBAudioDetectionSubsystem::OnBeatCallback(int32 Bar, int32 Beat, int32 Position, float Tempo,
                                                int32 TimeSignatureUpper, int32 TimeSignatureLower)
{	
	UpdateEventInformation(GetCurrentEventInformation(), Bar, Beat);
	TimeSinceLastBeat[EBeatType::Any] = 0.f;
	TimeSinceLastBeat[EBeatType::Whole] = 0.f;
	OnBeatOccurDelegate.Broadcast();
	
	if(GetCurrentEventInformation().bFinishSettingUp)
	{
		GetWorld()->GetTimerManager().SetTimer(HalfBeatTimerHandle, this, &UPBAudioDetectionSubsystem::OnHalfBeatCallback, GetCurrentEventInformation().BeatIntervals[EBeatType::Half]);
		GetWorld()->GetTimerManager().SetTimer(QuarterBeatTimerHandle, this, &UPBAudioDetectionSubsystem::OnQuarterBeatCallback, GetCurrentEventInformation().BeatIntervals[EBeatType::Quarter]);
	}
}

void UPBAudioDetectionSubsystem::OnHalfBeatCallback()
{
	OnHalfBeatOccurDelegate.Broadcast();
	TimeSinceLastBeat[EBeatType::Any] = 0.f;
	TimeSinceLastBeat[EBeatType::Half] = 0.f;
	GetWorld()->GetTimerManager().SetTimer(HalfBeatTimerHandle, this, &UPBAudioDetectionSubsystem::OnHalfBeatCallback, GetCurrentEventInformation().BeatIntervals[EBeatType::Half]);
}

void UPBAudioDetectionSubsystem::OnQuarterBeatCallback()
{
	OnQuarterBeatOccurDelegate.Broadcast();
	TimeSinceLastBeat[EBeatType::Any] = 0.f;
	TimeSinceLastBeat[EBeatType::Quarter] = 0.f;
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
	for (TPair<EBeatType, float>& Pair : TimeSinceLastBeat)
	{
		Pair.Value += InDeltaTime;
	}
}
