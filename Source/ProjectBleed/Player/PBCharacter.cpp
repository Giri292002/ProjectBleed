// Fill out your copyright notice in the Description page of Project Settings.


#include "PBCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectBleed/Systems/Locomotion/PBDashComponent.h"
#include "ProjectBleed/Systems/Interaction/PBInteractionComponent.h"
#include "ProjectBleed/Systems/Combat/PBCombatComponent.h"
#include "ProjectBleed/Libraries/CustomLogging.h"

DEFINE_LOG_CATEGORY(LogPBCharacter);

// Sets default values
APBCharacter::APBCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	DashComponent = CreateDefaultSubobject<UPBDashComponent>(TEXT("DashComponent"));

	InteractionComponent = CreateDefaultSubobject<UPBInteractionComponent>(TEXT("InteractionComponent"));

	CombatComponent = CreateDefaultSubobject<UPBCombatComponent>(TEXT("CombatComponent"));

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

void APBCharacter::Interact()
{
	if (!IsValid(InteractionComponent))
	{
		VSCREENMSG("Invalid Interaction Component");
	}
	InteractionComponent->Interact();
}

void APBCharacter::StartFire()
{
	if (!IsValid(CombatComponent))
	{
		V_LOG_ERROR(LogPBWeapon, "Invalid Combat Component");
	}
	CombatComponent->FireCurrentWeapon();
}

void APBCharacter::StopFire()
{
	if (!IsValid(CombatComponent))
	{
		V_LOG_ERROR(LogPBWeapon, "Invalid Combat Component");
	}
	CombatComponent->StopFireCurrentWeapon();
}
