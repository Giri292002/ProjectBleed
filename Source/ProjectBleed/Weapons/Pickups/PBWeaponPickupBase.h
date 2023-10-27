// All Rights belong to Studio WASD 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../PBWeaponBase.h"
#include "../../Systems/Interaction/PBInteractableInterface.h"
#include "Components/SphereComponent.h"

#include "PBWeaponPickupBase.generated.h"

UCLASS()
class PROJECTBLEED_API APBWeaponPickupBase : public AActor, public IPBInteractableInterface
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

	UPROPERTY(EditDefaultsOnly, Category = "Pickup | Weapon")
	TSubclassOf<APBWeaponBase> WeaponToGive = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup | Interaction")
	int InteractionPriority = 0;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, Category = "Pickup | Interaction")
	int GetInteractionPriority();
	virtual int GetInteractionPriority_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, Category = "Pickup | Interaction")
	void Interact(AActor* Interactor);
	virtual void Interact_Implementation(AActor* Interactor);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
