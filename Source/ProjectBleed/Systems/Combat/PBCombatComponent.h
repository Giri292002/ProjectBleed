// All Rights belong to Studio WASD 2023

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../Weapons/PBWeaponBase.h"
#include "PBCombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTBLEED_API UPBCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPBCombatComponent();

protected:
	UPROPERTY(BlueprintReadOnly)
	APBCharacter* PBCharacterOwner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	APBWeaponBase* CurrentWeapon = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<APBWeaponBase> DefaultWeapon = nullptr;
		
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	//Generate a weapon and attach it to the character
	// @param WeaponClass - The class of the weapon to generate
	// @param bOverrideDefaultAmmoCount - Default to false, Override the new weapon spawned with custom current ammo instead of using MaxAmmo from WeaponData
	// @param CurrentAmmo The Starting ammo to give to the weapon
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void GiveWeapon(TSubclassOf<APBWeaponBase> WeaponClass, bool bOverrideDefaultAmmoCount = false, int CurrentAmmo = 0);

	//Remove the current weapon from the character
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void RemoveWeapon();

	//Fire Current Weapon
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void FireCurrentWeapon();

	//Stop Firing Current Weapon
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StopFireCurrentWeapon();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ThrowWeapon();

	//Check if the character has a weapon
	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool HasWeapon() const { return CurrentWeapon != nullptr; }

	//Returns the current weapon
	UFUNCTION(BlueprintCallable, Category = "Combat")
	APBWeaponBase* GetCurrentWeapon() const { return CurrentWeapon; }

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
