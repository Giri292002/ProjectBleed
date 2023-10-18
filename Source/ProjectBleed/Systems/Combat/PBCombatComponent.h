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
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void GiveWeapon(TSubclassOf<APBWeaponBase> WeaponClass);

	//Fire Current Weapon
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void FireCurrentWeapon();

	//Stop Firing Current Weapon
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StopFireCurrentWeapon();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
