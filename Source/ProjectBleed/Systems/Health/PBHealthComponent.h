// All Rights belong to Studio WASD 2023

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PBHealthComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPBHealthComponent, Log, All)

class UPBHealthData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTBLEED_API UPBHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPBHealthComponent();

	UFUNCTION(BlueprintCallable, Category = "ProjectBleed |  Health")
	int GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintCallable, Category = "ProjectBleed | Health")
	int GetMaxHealth() const { return CurrentMaxHealth; }

protected:

	UPROPERTY(EditDefaultsOnly, Category = "ProjectBleed | Default")
	UPBHealthData* HealthData;

	UPROPERTY()
	int CurrentHealth;

	UPROPERTY()
	int CurrentMaxHealth;

	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;		
};
