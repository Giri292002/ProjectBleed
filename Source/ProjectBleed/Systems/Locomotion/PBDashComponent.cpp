#include "PBDashComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectBleed/Player/PBCharacter.h"

// Sets default values for this component's properties
UPBDashComponent::UPBDashComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	MovementMultiplier = 2.f;
}

void UPBDashComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<APBCharacter>(GetOwner());
	DashVelocity = OwnerCharacter->GetCharacterMovement()->GetMaxSpeed() * MovementMultiplier;
}

void UPBDashComponent::PerformDash(const FVector2D& DashDirection)
{
	if(OwnerCharacter->GetCharacterMovement()->IsFalling() ||OwnerCharacter->GetCharacterMovement()->IsFlying()) return;
	GetWorld()->GetSubsystem<UPBScoringSubsystem>()->AddToScore(ScoreData);
	
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("DASH Direction: %s"), *DashDirection.ToString()));
	const FVector DashDirection3D = (OwnerCharacter->GetCharacterDirection().RightVector * DashDirection.X + OwnerCharacter->GetCharacterDirection().ForwardVector * DashDirection.Y) * DashVelocity;
	ClampVector(DashDirection3D, FVector::ZeroVector, FVector(OwnerCharacter->GetCharacterMovement()->GetMaxSpeed(),OwnerCharacter->GetCharacterMovement()->GetMaxSpeed(),0.f));
	OwnerCharacter->LaunchCharacter(DashDirection3D, true, false);
}

