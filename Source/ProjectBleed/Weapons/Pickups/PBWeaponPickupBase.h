// All Rights belong to Studio WASD 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../PBWeaponBase.h"
#include "Components/SphereComponent.h"

#include "PBWeaponPickupBase.generated.h"

UCLASS()
class PROJECTBLEED_API APBWeaponPickupBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APBWeaponPickupBase();

protected:
	//This sphere overlap traces for the player
	UPROPERTY(EditDefaultsOnly)
	USphereComponent* SphereOverlap;

	//Only used for highlighting visual purposes
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* SphereMesh;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* WeaponStaticMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<APBWeaponBase> WeaponToGive = nullptr;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnComponentOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
