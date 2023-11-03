// All Rights belong to Studio WASD 2023

#pragma once

#include "CoreMinimal.h"
#include "Data/PBWeaponDataBase.h"
#include "GameFramework/Actor.h"
#include "ProjectBleed/Systems/Scoring/PBScoringSubsystem.h"
#include "ProjectBleed/Player/PBCharacter.h"
#include "ProjectBleed/Systems/PBThrowableObjectComponent.h"
#include "PBWeaponBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPBWeapon, Log, All);

class APBPlayerController;
class UPBBulletHitReactionComponent;
class UCapsuleComponent;
class USceneComponent;
class UPBThrowableObjectComponent;

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

	UPROPERTY(EditDefaultsOnly, Category = "ProjectBleed| Weapons | Root")
	UCapsuleComponent* CapsuleCollision = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectBleed| Weapons | Weapon")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectBleed | Weapons  | Weapon")
	UPBThrowableObjectComponent* ThrowableObjectComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectBleed| Weapons | Data")
	UPBWeaponDataBase* WeaponData = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "ProjectBleed| Weapons | Data")
	int CurrentAmmo = 0;

	UPROPERTY()
	FTimerHandle FireTimerHandle;

	UPROPERTY()
	int CurrentBurstCount = 0;

	UPROPERTY()
	APBPlayerController* PBPlayerController = nullptr;

	UPROPERTY()
	FTimerHandle BurstFireTimerHandle;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void InternalFire();

	UFUNCTION()
	void InternalBurstFire();

	UFUNCTION()
	void OnNoAmmo();

	UFUNCTION()
	bool PerformLineTrace(FHitResult& OutHitResult);

	/**
	* @brief Spawns a bullet projectile
	*/
	UFUNCTION()
	void SpawnBulletProjectile(const FHitResult& InHitResult);

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

	UFUNCTION()
	int GetCurrentAmmo() const { return CurrentAmmo; }

	//Returns the weapon data
	UFUNCTION()
	UPBWeaponDataBase* GetWeaponData() const { return WeaponData; }

	UFUNCTION()
	bool HasAmmo() const { return CurrentAmmo > 0; }

	/**
	* @brief Reduces the ammo by the specified amount 
	* @param AmmoToReduce The amount of ammo to reduce
	*/
	UFUNCTION()
	void ReduceAmmo(int AmmoToReduce = 1);

	/**
	* @brief Adds the ammo by the specified amount
	* @param AmmoToAdd The amount of ammo to add
	*/
	UFUNCTION()
	void AddAmmo(int AmmoToAdd = 1);

};
