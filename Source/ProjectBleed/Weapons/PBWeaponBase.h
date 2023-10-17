// All Rights belong to Studio WASD 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* EquipAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TSubclassOf<UAnimInstance> AnimationLayer = nullptr;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnEquip();

	UFUNCTION()
	void OnUnEquip();

	UFUNCTION()
	void OnFire();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
