// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ProjectBleed/UI/PBCrosshair.h"
#include "PBHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBLEED_API APBHUD : public AHUD
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UPBCrosshair> CrosshairWidgetClass;
	UUserWidget* CrosshairWidget;

	virtual void BeginPlay() override;
};
