// Fill out your copyright notice in the Description page of Project Settings.


#include "PBCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectBleed/Systems/Locomotion/PBDashComponent.h"

class UPawnMovementComponent;
// Sets default values
APBCharacter::APBCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	DashComponent = CreateDefaultSubobject<UPBDashComponent>(TEXT("DashComponent"));

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	SpringArmComponent->bInheritYaw = false;
	SpringArmComponent->bInheritPitch = false;
	SpringArmComponent->bInheritRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = 800.f;
}

// Called when the game starts or when spawned
void APBCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APBCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APBCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UInputMappingContext* APBCharacter::GetInputMappingContext_Implementation()
{
	return InputMappingContext;
}

FVector APBCharacter::GetCharacterDirection() const
{
	return FVector(SpringArmComponent->GetComponentRotation().Vector().ForwardVector.Length(),SpringArmComponent->GetComponentRotation().Vector().RightVector.Length(), 0.f); 
}

void APBCharacter::Move(const FInputActionValue& ActionValue)
{
	MovementDirection = ActionValue.Get<FVector2D>();

	AddMovementInput(SpringArmComponent->GetComponentRotation().Vector().ForwardVector, MovementDirection.Y);
	AddMovementInput(SpringArmComponent->GetComponentRotation().Vector().RightVector, MovementDirection.X);
}

void APBCharacter::Dash()
{
	DashComponent->PerformDash(MovementDirection);
}
