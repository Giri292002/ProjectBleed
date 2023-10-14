// Fill out your copyright notice in the Description page of Project Settings.


#include "PBAudioPlayTester.h"
#include "PBAudioDetectionSubsystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APBAudioPlayTester::APBAudioPlayTester()
{
}

// Called when the game starts or when spawned
void APBAudioPlayTester::BeginPlay()
{
	Super::BeginPlay();	
	PlayAudio();
}

void APBAudioPlayTester::OnBeat()
{
	if(bPrintBeatDebug)
	{
		GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::Green,TEXT(" APBAudioPlayTester::OnBeat() | BEAT"));
	}
}

void APBAudioPlayTester::OnHalfBeat()
{
	if(bPrintHalfBeatDebug)
	{
		GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::Red,TEXT(" APBAudioPlayTester::OnBeat() | HALF BEAT"));
	}
}

void APBAudioPlayTester::OnQuarterBeat()
{
	if(bPrintQuarterBeatDebug)
	{
		GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::Blue,TEXT(" APBAudioPlayTester::OnBeat() | QUARTER BEAT"));
	}
}

void APBAudioPlayTester::PlayAudio()
{
	UPBAudioDetectionSubsystem* AudioDetectionSubsystem = GetWorld()->GetSubsystem<UPBAudioDetectionSubsystem>();
	AudioDetectionSubsystem->ProcessAndPlayAudio(MusicToPlay);
	AudioDetectionSubsystem->OnBeatOccurDelegate.AddDynamic(this, &APBAudioPlayTester::OnBeat);
	AudioDetectionSubsystem->OnHalfBeatOccurDelegate.AddDynamic(this, &APBAudioPlayTester::OnHalfBeat);
	AudioDetectionSubsystem->OnQuarterBeatOccurDelegate.AddDynamic(this, &APBAudioPlayTester::OnQuarterBeat);
}