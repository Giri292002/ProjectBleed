// All Rights belong to Studio WASD 2023


#include "PBInteractionComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PBInteractableInterface.h"
#include "ProjectBleed/Player/PBCharacter.h"

DEFINE_LOG_CATEGORY(LogPBInteractionComponent);

// Sets default values for this component's properties
UPBInteractionComponent::UPBInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPBInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	PBOwnerCharacter = Cast<APBCharacter>(GetOwner());
}

void UPBInteractionComponent::Interact()
{
	if (!IsValid(PBOwnerCharacter))
	{
		UE_LOG(LogPBInteractionComponent, Error, TEXT("Invalid PBOwnerCharacter"));
		return;
	}

	TArray<FHitResult> OutResult;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(TraceRadius);
	FCollisionQueryParams CollisionQueryParams = FCollisionQueryParams();
	CollisionQueryParams.AddIgnoredActor(PBOwnerCharacter);
	CollisionQueryParams.bDebugQuery = true;
	const FVector& TraceLocation = PBOwnerCharacter->GetCapsuleComponent()->GetComponentLocation() - FVector(0.f,0.f, PBOwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(PBOwnerCharacter);

	
	if (UKismetSystemLibrary::SphereTraceMulti(GetWorld(), TraceLocation, TraceLocation, TraceRadius, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), false,
		ActorsToIgnore, EDrawDebugTrace::ForDuration, OutResult, true))
	{
		int HighestPriority = -1;
		AActor* Candidate = nullptr;
		for (const FHitResult& HitResult : OutResult)
		{
			//Get the actor with the highest priority and interact with it
			if (HitResult.GetActor()->GetClass()->ImplementsInterface(UPBInteractableInterface::StaticClass()))
			{
				int Priority = IPBInteractableInterface::Execute_GetInteractionPriority(HitResult.GetActor());
				if (Priority > HighestPriority)
				{
					HighestPriority = Priority;
					Candidate = HitResult.GetActor();
				}
			}
		}

		if (!IsValid(Candidate))
		{
			UE_LOG(LogPBInteractionComponent, Log, TEXT("No Valid Candidate"));
			return;
		}

		UE_LOG(LogPBInteractionComponent, Log, TEXT("Called Interact on: %s"), *Candidate->GetName())
		IPBInteractableInterface::Execute_Interact(Candidate, PBOwnerCharacter);
	}
}

