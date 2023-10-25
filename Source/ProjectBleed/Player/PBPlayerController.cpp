// Fill out your copyright notice in the Description page of Project Settings.


#include "PBPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectBleed/Input/PBInput.h"

void APBPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (InPawn->GetClass()->ImplementsInterface(UPBInput::StaticClass()))
	{
		if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IPBInput::Execute_GetInputMappingContext(InPawn), 0);
		}
	}

	if (auto* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, InPawn, TEXT("Move"));
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, InPawn, TEXT("Dash"));
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, InPawn, TEXT("Interact"));
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, InPawn, TEXT("Fire"));
	}
}

void APBPlayerController::OnUnPossess()
{
	if (GetPawn()->GetClass()->ImplementsInterface(UPBInput::StaticClass()))
	{
		if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(IPBInput::Execute_GetInputMappingContext(GetPawn()));
		}
	}
	Super::OnUnPossess();
}

void APBPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateNewControlRotationToCursor();
	UpdateControlRotation();
}

void APBPlayerController::UpdateNewControlRotationToCursor()
{
	if (!IsValid(GetPawn()))
	{
		return;
	}
	FHitResult HitResult;
	if (GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
	{
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
			GetPawn()->GetActorLocation(), HitResult.ImpactPoint);

		NewControlRotation.Yaw = LookAtRotation.Yaw;
	}
}

void APBPlayerController::UpdateControlRotation()
{
	SetControlRotation(UKismetMathLibrary::RInterpTo(GetControlRotation(), NewControlRotation,
	                                                 UGameplayStatics::GetWorldDeltaSeconds(this), 10.f));
}
