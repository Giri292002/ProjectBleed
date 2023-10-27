// All Rights belong to Studio WASD 2023

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Data/PBBulletFXData.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "PBBulletHitReactionComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTBLEED_API UPBBulletHitReactionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPBBulletHitReactionComponent();

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectBleed | Weapons | FX")
	UPBBulletFXData* BulletFXData;

public:	

	/**
	* @brief Plays the impact FX based on the hit result
	* @param HitResult The hit result from the bullet
	*/
	UFUNCTION()
	void PlayImpactFX(const FHitResult& HitResult);
};
