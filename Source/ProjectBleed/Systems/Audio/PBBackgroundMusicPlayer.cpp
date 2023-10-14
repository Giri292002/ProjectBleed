// Fill out your copyright notice in the Description page of Project Settings.


#include "PBBackgroundMusicPlayer.h"

#include "FMODAudioComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APBBackgroundMusicPlayer::APBBackgroundMusicPlayer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BackgroundMusicComponent = CreateDefaultSubobject<UFMODAudioComponent>("Background Score");
	BackgroundMusicComponent->SetupAttachment(RootComponent);
	BackgroundMusicComponent->bEnableTimelineCallbacks = true;

	GhostBackgroundMusicComponent = CreateDefaultSubobject<UFMODAudioComponent>("Ghost Background Score");
	GhostBackgroundMusicComponent->SetupAttachment(RootComponent);
	GhostBackgroundMusicComponent->bEnableTimelineCallbacks = true;
}

// Called when the game starts or when spawned
void APBBackgroundMusicPlayer::BeginPlay()
{
	Super::BeginPlay();
}


