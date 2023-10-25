// All Rights belong to Studio WASD 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectBleed/Systems/Scoring/PBScoringSubsystem.h"
#include "Data/PBWeaponDataBase.h"
#include "ProjectBleed/Player/PBCharacter.h"
#include "PBWeaponBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPBWeapon, Log, All);

UCLASS()
class PROJECTBLEED_API APBWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APBWeaponBase();

protected:

	UPROPERTY(BlueprintReadOnly)	
	APBCharacter* PBOwnerCharacter = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectBleed| Weapons | Root")
	USceneComponent* Root = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectBleed| Weapons | Weapon")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectBleed| Weapons | Data")
	UPBWeaponDataBase* WeaponData = nullptr;

	UPROPERTY()
	FTimerHandle FireTimerHandle;

	UPROPERTY()
	int CurrentBurstCount = 0;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void InternalFire();

	UFUNCTION()
	void InternalBurstFire();
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Called when the weapon is equipped
	UFUNCTION()
	virtual void Equip();

	//Called when the weapon is unequipped
	UFUNCTION()
	virtual void UnEquip();

	//Called when the weapon is fired
	UFUNCTION()
	virtual void Fire();

	UFUNCTION()
	virtual void StopFire();
};
