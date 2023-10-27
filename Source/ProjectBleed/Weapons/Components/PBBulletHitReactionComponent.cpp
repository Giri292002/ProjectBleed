// All Rights belong to Studio WASD 2023


#include "PBBulletHitReactionComponent.h"
#include "ProjectBleed/Libraries/CustomLogging.h"
#include "ProjectBleed/Weapons/PBWeaponBase.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values for this component's properties
UPBBulletHitReactionComponent::UPBBulletHitReactionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPBBulletHitReactionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UPBBulletHitReactionComponent::PlayImpactFX(const FHitResult& HitResult)
{
	if (!IsValid(BulletFXData))
	{
		UE_LOG(LogPBWeapon, Warning, TEXT("No Bullet FX Data found"));
		return;
	}

	if (HitResult.bBlockingHit)
	{
		//Play Random Particle System based on Effect Type
		const TEnumAsByte<EPhysicalSurface> PhysicalSurface = HitResult.PhysMaterial.Get()->SurfaceType;
		const TArray<UNiagaraSystem*> NiagaraSystems = BulletFXData->SurfaceBulletImpactFXPair.FindRef(PhysicalSurface).ParticleSystem;

		if(NiagaraSystems.IsEmpty())
		{
			UE_LOG(LogPBWeapon, Warning, TEXT("No Particle System found for Surface Type: %s"), *UEnum::GetValueAsString(PhysicalSurface));
			return;
		}

		const int RandomIndex = FMath::RandRange(0, NiagaraSystems.Num() - 1);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraSystems[RandomIndex], HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation(), FVector(2.f,2.f,2.f));
	}
}

