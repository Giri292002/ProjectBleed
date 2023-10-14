// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PBBackgroundMusicPlayer.generated.h"

class UFMODAudioComponent;

//This is created by the PBAudioDetectionSubsystem and only used to play music and bind events to callbacks.
UCLASS(NotBlueprintable)
class PROJECTBLEED_API APBBackgroundMusicPlayer : public AActor
{
	GENERATED_BODY()

public:
	UFMODAudioComponent* GetBackgroundMusicComponent() const { return BackgroundMusicComponent; }
	UFMODAudioComponent* GetGhostBackgroundMusicComponent() const { return GhostBackgroundMusicComponent; }
	float GetMusicDelay() const { return  MusicDelay; }
	
public:	
	// Sets default values for this actor's properties
	APBBackgroundMusicPlayer();

	
protected:
	//This is the time that the ghost background music will be ahead for. BGMComponent will always be 1.5f behind GhostBGMComponent
	UPROPERTY(EditAnywhere, Category = "Audio")
	float MusicDelay = 1.f;
	
	//This runs the background score for the game. The PBAudioDetection Subsystem uses this to play events. This is what the players will here
	UPROPERTY(EditDefaultsOnly, Category= "Audio")
	UFMODAudioComponent* BackgroundMusicComponent;

	//This runs the background score for the game. The PBAudioDetection Subsystem uses this to play events. This is what UI events
	//or anything else that needs to know ahead what is going to get played
	UPROPERTY(EditDefaultsOnly, Category= "Audio")
	UFMODAudioComponent* GhostBackgroundMusicComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
