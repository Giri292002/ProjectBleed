// All Rights belong to Studio WASD 2023


#include "PBAICharacter.h"
#include "PBAIControllerBase.h"
#include "ProjectBleed/Systems/Health/PBHealthComponent.h"

// Sets default values
APBAICharacter::APBAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UPBHealthComponent>(TEXT("HealthComponent"));

	//Setup AI Controller
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = APBAIControllerBase::StaticClass();
}

// Called when the game starts or when spawned
void APBAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APBAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APBAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

