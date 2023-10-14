// Fill out your copyright notice in the Description page of Project Settings.


#include "PBBeatCrosshair.h"
#include "Components/Image.h"

void UPBBeatCrosshair::NativeConstruct()
{	
	Super::NativeConstruct();

	// Store the initial position of the widget
	InitialPosition = GetRenderTransform().Translation;
}

void UPBBeatCrosshair::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if(CurveTimeline.IsPlaying())
	{
		CurveTimeline.TickTimeline(InDeltaTime);
	}	
}

// Function to stop the widget movement
void UPBBeatCrosshair::StopWidgetMovement()
{
	// Set the widget's position to the target position
	RemoveFromParent();
}

void UPBBeatCrosshair::SetBeatInfo(const EBeatType& InBeatType, const EBeatSpawnDirection& InBeatDirection, const float InMovementDuration)
{
	MovementDuration = InMovementDuration;
	BeatType = InBeatType;
	BeatSpawnDirection = InBeatDirection;
	const uint8 BeatSwitchValue = (uint8)BeatType;
	BeatArrow->GetDynamicMaterial()->SetScalarParameterValue("CrosshairType", BeatSwitchValue);
	BeatArrow->GetDynamicMaterial()->SetScalarParameterValue("BeatFacingDirection", BeatSpawnDirection == EBeatSpawnDirection::Right ? 1.f : -1.f);

	if(CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
		CurveTimeline.AddInterpFloat(CurveFloat, TimelineProgress, FName(TEXT("EFFECTFADE")));
		CurveTimeline.SetPlayRate(1.f/MovementDuration);
		CurveTimeline.Play();
	}
}

void UPBBeatCrosshair::TimelineProgress(float Value)
{
	const FVector2D CurrentPosition = FMath::Lerp(InitialPosition, TargetPosition, Value);
	SetRenderTranslation(CurrentPosition);
}
