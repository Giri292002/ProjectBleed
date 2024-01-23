// All Rights belong to Studio WASD 2023


#include "PBScoringWidget.h"

#include "Components/TextBlock.h"
#include "ProjectBleed/Systems/Audio/PBAudioDetectionSubsystem.h"
#include "ProjectBleed/Systems/Scoring/PBScoringSubsystem.h"


void UPBScoringWidget::NativeConstruct()
{
	UPBScoringSubsystem* const PBScoringSubsystem = GetWorld()->GetSubsystem<UPBScoringSubsystem>();

	if (!ensureAlwaysMsgf(PBScoringSubsystem, TEXT("PBScoringSubsystem is invalid")))
	{
		return;
	}

	PBScoringSubsystem->OnScoreUpdate.AddDynamic(this, &UPBScoringWidget::OnScoreUpdate);

	UPBAudioDetectionSubsystem* const PBAudioDetectionSubsystem = GetWorld()->GetSubsystem<UPBAudioDetectionSubsystem>();

	if (!ensureAlwaysMsgf(PBAudioDetectionSubsystem, TEXT("PBAudioDetectionSubsystem is invalid")))
	{
		return;
	}

	OnScoreUpdate(0);
	PBAudioDetectionSubsystem->OnBeatOccurDelegate.AddDynamic(this, &UPBScoringWidget::OnWholeBeat);
}


void UPBScoringWidget::OnScoreUpdate(int NewScore)
{
	ScoreText->SetText(FText::FromString(FString::FromInt(NewScore)));
}

void UPBScoringWidget::OnWholeBeat()
{
	PlayAnimation(OnBeatAnimation);
}
