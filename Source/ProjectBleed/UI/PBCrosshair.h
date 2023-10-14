// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PBBeatCrosshair.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/Overlay.h"

#include "PBCrosshair.generated.h"

/**
 * 
 */
enum class EBeatType : uint8;
UCLASS()
class PROJECTBLEED_API UPBCrosshair : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* Crosshair;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UOverlay* Overlay;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPBBeatCrosshair> BeatClass;

	UPROPERTY(EditDefaultsOnly, Category="Audio")
	float BeatSpawnOffset = 100.f;

	//Set from audio subsystem
	UPROPERTY(BlueprintReadOnly, Category="Audio")
	float MusicDelay = 5.f;

	UFUNCTION(BlueprintImplementableEvent)
	void OnBeatOccur();

	UFUNCTION(BlueprintImplementableEvent)
	void OnHalfBeatOccur();

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY()
	UPBAudioDetectionSubsystem* AudioDetectionSubsystem;
	
	UFUNCTION()
	void OnGhostBeatOccur();
	UFUNCTION()
	void OnGhostHalfBeatOccur();
	UFUNCTION()
	void OnGhostQuarterBeatOccur();
	UFUNCTION()
	void CreateBeatWidget(const EBeatType InBeatType, const EBeatSpawnDirection BeatSpawnDirection);

	bool bDonce = false;
};
