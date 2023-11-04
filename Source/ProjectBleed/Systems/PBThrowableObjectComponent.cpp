// All Rights belong to Studio WASD 2023


#include "PBThrowableObjectComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "ProjectBleed/Libraries/CustomLogging.h"
#include "ProjectBleed/Player/PBPlayerController.h"

DEFINE_LOG_CATEGORY(LogPBThrowableComponent);

// Sets default values for this component's properties
UPBThrowableObjectComponent::UPBThrowableObjectComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = false;
	InitialSpeed = 3000.f;
	//Unlimited Max Speed
	MaxSpeed = 0.f;
	ProjectileGravityScale = 1.f;
	bShouldBounce = true;
}


// Called when the game starts
void UPBThrowableObjectComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPBThrowableObjectComponent::Throw()
{
	if (GetOwner() == nullptr)
	{
		V_LOG_ERROR(LogPBThrowableComponent, TEXT("Throwable Object Component has no owner"));
		return;
	}
	//If the object is attached to another actor, detach it

	if (GetOwner()->GetParentActor())
	{
		GetOwner()->GetParentActor()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}

	if (ThrowEffect == nullptr)
	{
		V_LOG_ERROR(LogPBThrowableComponent, TEXT("No Throw Effect"));
		return;
	}

	UNiagaraComponent* SpawnedSystem = UNiagaraFunctionLibrary::SpawnSystemAttached(ThrowEffect, GetOwner()->GetRootComponent(), NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true, true, ENCPoolMethod::AutoRelease, true);

	if (SpawnedSystem == nullptr)
	{
		V_LOG_ERROR(LogPBThrowableComponent, TEXT("Failed to spawn throw effect"));
		return;
	}

	const FVector& ShootDirection = Cast<APBPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->GetMouseWorldDirection(GetOwner()->GetActorLocation());

	Velocity = ShootDirection * InitialSpeed;
	Activate(true);
}

