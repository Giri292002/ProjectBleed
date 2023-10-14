// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TimelineComponent.h"
#include "ProjectBleed/Systems/Audio/PBAudioDetectionSubsystem.h"
#include "PBBeatCrosshair.generated.h"

class UImage;
class UCurveFloat;

UENUM()
enum class EBeatSpawnDirection : uint8
{
	Left = 0,
	Right = 1
};

UCLASS()
class PROJECTBLEED_API UPBBeatCrosshair : public UUserWidget
{
	GENERATED_BODY()

public:
	//The ghost music delay that occurs. Can be found in APBBackgroundMusicPlayer
	UPROPERTY(BlueprintReadOnly, Category="Audio")
	float MovementDuration;
	
	//Starting position of this widget
	UPROPERTY()
	FVector2D InitialPosition;

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
	EBeatType BeatType;

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
	EBeatSpawnDirection BeatSpawnDirection = EBeatSpawnDirection::Right;

	UFUNCTION()
	void SetBeatInfo(const EBeatType& InBeatType, const EBeatSpawnDirection& InBeatDirection, const float InMovementDuration);

	/** Function which gets called from the Timeline on a Tick */
	UFUNCTION()
	void TimelineProgress(float Value);

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (BindWidget))
	UImage* BeatArrow;

	FTimeline CurveTimeline;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* CurveFloat;
	
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
private:
	
	//The final position this beat needs to go to before despawning
	UPROPERTY()
	FVector2D TargetPosition = FVector2D().ZeroVector;
	
	UFUNCTION()
	void StopWidgetMovement();

};
