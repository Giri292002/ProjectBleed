// All Rights belong to Studio WASD 2023

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Chaos/ChaosEngineInterface.h"
#include "NiagaraSystem.h"
#include "PBBulletFXData.generated.h"

/**
 * 
 */
 //Cant Add Array to a TMap, so adding a strcut as a work around
USTRUCT(BlueprintType)
struct FBulletImpactFX
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UNiagaraSystem*> ParticleSystem;
};

UCLASS()
class PROJECTBLEED_API UPBBulletFXData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, meta = (ForceInlineRow))
	TMap<TEnumAsByte<EPhysicalSurface>, FBulletImpactFX> SurfaceBulletImpactFXPair;
};

	
