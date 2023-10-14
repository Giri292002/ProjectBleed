// Fill out your copyright notice in the Description page of Project Settings.


#include "PBCrosshair.h"
#include "ProjectBleed/Systems/Audio/PBAudioDetectionSubsystem.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UPBCrosshair::NativeConstruct()
{
	Super::NativeConstruct();
	AudioDetectionSubsystem = GetWorld()->GetSubsystem<UPBAudioDetectionSubsystem>();
	AudioDetectionSubsystem->OnGhostBeatOccurDelegate.AddDynamic(this, &UPBCrosshair::OnGhostBeatOccur);
	AudioDetectionSubsystem->OnGhostHalfBeatOccurDelegate.AddDynamic(this, &UPBCrosshair::OnGhostHalfBeatOccur);
	AudioDetectionSubsystem->OnGhostQuarterBeatOccurDelegate.AddDynamic(this, &UPBCrosshair::OnGhostQuarterBeatOccur);
	AudioDetectionSubsystem->OnBeatOccurDelegate.AddDynamic(this, &UPBCrosshair::OnBeatOccur);
	AudioDetectionSubsystem->OnHalfBeatOccurDelegate.AddDynamic(this, &UPBCrosshair::OnHalfBeatOccur);
}

void UPBCrosshair::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	const FVector2d MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	Overlay->SetRenderTranslation(MousePosition);
}

void UPBCrosshair::OnGhostBeatOccur()
{
	//if(bDonce) return;
	CreateBeatWidget(EBeatType::Whole, EBeatSpawnDirection::Right);
	CreateBeatWidget(EBeatType::Whole, EBeatSpawnDirection::Left);
	//bDonce = true;
}

void UPBCrosshair::OnGhostHalfBeatOccur()
{
	CreateBeatWidget(EBeatType::Half, EBeatSpawnDirection::Right);
	CreateBeatWidget(EBeatType::Half, EBeatSpawnDirection::Left);
}

void UPBCrosshair::OnGhostQuarterBeatOccur()
{
	//CreateBeatWidget(EBeatType::Quarter);
}

void UPBCrosshair::CreateBeatWidget(const EBeatType InBeatType, const EBeatSpawnDirection BeatSpawnDirection)
{
	FVector2D NewSpawn = FVector2D(BeatSpawnOffset, 0.f);
	if(BeatSpawnDirection == EBeatSpawnDirection::Left)
	{
		NewSpawn *= FVector2D(-1.f,1.f);
	}
	MusicDelay = AudioDetectionSubsystem->GetMusicDelay();
	UPBBeatCrosshair* SpawnedBeat = CreateWidget<UPBBeatCrosshair>(GetWorld(), BeatClass);	
	SpawnedBeat->InitialPosition = NewSpawn; 
	SpawnedBeat->MovementDuration = MusicDelay;
	SpawnedBeat->SetBeatInfo(InBeatType, BeatSpawnDirection, MusicDelay);
	SpawnedBeat->SetRenderTranslation(NewSpawn);
	Overlay->AddChildToOverlay(SpawnedBeat);	
}

