// All Rights belong to Studio WASD 2023


#include "PBScoringSubsystem.h"
#include "ProjectBleed/Systems/Audio/PBAudioDetectionSubsystem.h"
DEFINE_LOG_CATEGORY(LogPBScoringSubsystem);

void UPBScoringSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SetupSubsystem();
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

void UPBScoringSubsystem::SetupSubsystem()
{
	UE_LOG(LogPBScoringSubsystem, Log, TEXT("Initalized PB Scoring Subsystem"));
}

void UPBScoringSubsystem::AddToScore(const UPBScoreData* InScore)
{
	if(!IsValid(InScore))
	{
		UE_LOG(LogPBScoringSubsystem, Error, TEXT("Incoming Score Data is NULL"));
		return;
	}
	UPBAudioDetectionSubsystem* AudioDetectionSubsystem = GetWorld()->GetSubsystem<UPBAudioDetectionSubsystem>();
	float OutAccuracy;
	if(AudioDetectionSubsystem->WasOnBeat(OutAccuracy))
	{		
		UE_LOG(LogPBScoringSubsystem, Log, TEXT("Was on beat with accuracy: %f"), OutAccuracy);
	}
	else
	{
		UE_LOG(LogPBScoringSubsystem, Log, TEXT("Was NOT on beat with accuracy: %f"), OutAccuracy);
	}
	const float ScoreToAdd = FMath::Clamp(InScore->ScoreToAdd * OutAccuracy, 0, InScore->ScoreToAdd);
	//This will later be modified to be reduced based on how delayed or how early you pressed
	const int FinalAddedScore = FMath::TruncToInt(ScoreToAdd);
	
	BaseScore += FinalAddedScore;
	CurrentScore = BaseScore * CurrentScoreMultiplier;
	UE_LOG(LogPBScoringSubsystem, Log, TEXT("Current Score is: %i"), FinalAddedScore, CurrentScore);
}
