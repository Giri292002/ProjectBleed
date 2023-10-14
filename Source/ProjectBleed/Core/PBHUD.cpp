// Fill out your copyright notice in the Description page of Project Settings.


#include "PBHUD.h"

#include "Blueprint/UserWidgetBlueprint.h"

void APBHUD::BeginPlay()
{
	Super::BeginPlay();

	if (CrosshairWidgetClass != nullptr)
	{
		CrosshairWidget = CreateWidget<UPBCrosshair>(GetWorld(), CrosshairWidgetClass);
		CrosshairWidget->AddToViewport();
	}
}
