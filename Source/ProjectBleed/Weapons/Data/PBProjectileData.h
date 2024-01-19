// All Rights belong to Studio WASD 2023

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PBProjectileData.generated.h"

class APBProjectile;

UCLASS()
class PROJECTBLEED_API UPBProjectileData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<APBProjectile> ProjectileClass = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (MinClamp = 0))
	int WeaponDamage = 10;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	float ProjectileSpeed = 4000.f;
};
