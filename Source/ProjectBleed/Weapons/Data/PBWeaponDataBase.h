// All Rights belong to Studio WASD 2023

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../../PBProjectENUMS.h"
#include "PBWeaponDataBase.generated.h"

class APBProjectile;
class UPBScoreData;

UCLASS()
class PROJECTBLEED_API UPBWeaponDataBase : public UDataAsset
{
	GENERATED_BODY()

	public:
		UPROPERTY(EditDefaultsOnly, Category = "Default")
		EFireMode WeaponFireMode = EFireMode::SemiAuto;

		UPROPERTY(EditDefaultsOnly, Category = "Default", meta=(MinClamp = 0))
		int MaxAmmo = 10;

		UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (MinClamp = 0))
		int WeaponDamage = 10;

		UPROPERTY(EditDefaultsOnly, Category = "Default")
		bool bUseDynamicFireRate = false;

		UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (EditCondition = "bUseDynamicFireRate", EditConditionHides))
		EBeatType DynamicFireRateMode = EBeatType::Half;

		UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (MinClamp = 0.f, EditCondition = "bUseDynamicFireRate == false && (WeaponFireMode == EFireMode::Burst || WeaponFireMode == EFireMode::FullAuto)", EditConditionHides))
		float FireRate = 0.1f;

		UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (MinClamp = 0, EditCondition = "WeaponFireMode == EFireMode::Burst", EditConditionHides))
		int BurstCount = 0;

		UPROPERTY(EditDefaultsOnly, Category = "Projectile")
		TSubclassOf<APBProjectile> ProjectileClass = nullptr;

		UPROPERTY(EditDefaultsOnly, Category = "Default")
		float WeaponRange = 1000.f;	

		UPROPERTY(EditDefaultsOnly, Category = "Default")
		float ProjectileSpeed = 4000.f;

		UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* EquipAnimation;

		UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* WeaponFireAnimation;

		UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* CharacterFireAnimation;
		
		UPROPERTY(EditDefaultsOnly, Category= "Animation")
		UAnimMontage* EmptyMagAnimation;

		UPROPERTY(EditDefaultsOnly, Category = "Animation")
		TSubclassOf<UAnimInstance> AnimationLayer = nullptr;

		UPROPERTY(EditDefaultsOnly, Category = "Scoring")
		UPBScoreData* ScoreData = nullptr;
};
