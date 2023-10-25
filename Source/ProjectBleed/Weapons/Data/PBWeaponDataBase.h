// All Rights belong to Studio WASD 2023

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../../Systems/Scoring/PBScoreData.h"
#include "PBWeaponDataBase.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EFireMode : uint8
{
	SemiAuto, //Fires once per click
	FullAuto, //Fires while the mouse is held down
	Burst //Fires a set amount of bullets per click
};
	

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

		UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (MinClamp = 0.f, EditCondition = "WeaponFireMode == EFireMode::Burst || WeaponFireMode == EFireMode::FullAuto", EditConditionHides))
		float FireRate = 0.1f;

		UPROPERTY(EditDefaultsOnly, Category = "Default", meta = (MinClamp = 0, EditCondition = "WeaponFireMode == EFireMode::Burst", EditConditionHides))
		int BurstCount = 0;

		UPROPERTY(EditDefaultsOnly, Category = "Default")
		float WeaponRange = 1000.f;	

		UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* EquipAnimation;

		UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* WeaponFireAnimation;

		UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* CharacterFireAnimation;

		UPROPERTY(EditDefaultsOnly, Category = "Animation")
		TSubclassOf<UAnimInstance> AnimationLayer = nullptr;

		UPROPERTY(EditDefaultsOnly, Category = "Scoring")
		UPBScoreData* ScoreData = nullptr;
};
