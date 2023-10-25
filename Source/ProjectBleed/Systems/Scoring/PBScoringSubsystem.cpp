// All Rights belong to Studio WASD 2023


#include "PBScoringSubsystem.h"

#include "FMODEvent.h"
#include "FMODBlueprintStatics.h"
#include <ProjectBleed/Libraries/CustomLogging.h>
#include "ProjectBleed/Systems/Audio/PBAudioDetectionSubsystem.h"
#include <ProjectBleed/Settings/PBScoringSubsystemSettings.h>

DEFINE_LOG_CATEGORY(LogPBScoringSubsystem);

void UPBScoringSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	V_LOG(LogPBScoringSubsystem, TEXT("Initalized PB Scoring Subsystem"));

	const UPBScoringSubsystemSettings* Settings = GetDefault<UPBScoringSubsystemSettings>();
	if (IsValid(Settings))
	{
		OnAccurateHitEvent = Settings->OnAccurateHitEventPath.LoadSynchronous();
	}
}

TStatId UPBScoringSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UPBScoringSubsystem, STATGROUP_Tickables);
}

bool UPBScoringSubsystem::ShouldCreateSubsystem(UObject* Outer) const
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

void UPBScoringSubsystem::AddToScore(const UPBScoreData* InScore)
{
	if(!IsValid(InScore))
	{
		V_LOG_ERROR(LogPBScoringSubsystem, TEXT("Incoming Score Data is NULL"));
		return;
	}

	UPBAudioDetectionSubsystem* AudioDetectionSubsystem = GetWorld()->GetSubsystem<UPBAudioDetectionSubsystem>();
	float OutAccuracy;
	if(AudioDetectionSubsystem->WasOnBeat(OutAccuracy))
	{		
		V_LOGF(LogPBScoringSubsystem, TEXT("Was on beat with accuracy: %f"), OutAccuracy);

		if(!IsValid(OnAccurateHitEvent))
		{
			V_LOG_ERROR(LogPBScoringSubsystem, TEXT("OnAccurateHitEvent is NULL"));
			return;
		}
		else
		{
			//Play Accurate Hit Sound
			UFMODBlueprintStatics::PlayEvent2D(GetWorld(), OnAccurateHitEvent, true);
		}		
	}
	else
	{
		V_LOGF(LogPBScoringSubsystem, TEXT("Was NOT on beat with accuracy: %f"), OutAccuracy);
	}

	const float ScoreToAdd = FMath::Clamp(InScore->ScoreToAdd * OutAccuracy, 0, InScore->ScoreToAdd);
	const int FinalAddedScore = FMath::TruncToInt(ScoreToAdd);
	
	BaseScore += FinalAddedScore;
	CurrentScore = BaseScore * CurrentScoreMultiplier;
	V_LOGF(LogPBScoringSubsystem, TEXT("Current Score is: %i"), CurrentScore);
}
