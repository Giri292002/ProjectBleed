// All Rights belong to Studio WASD 2023

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PBProjectile.generated.h"

class UPBBulletHitReactionComponent;
class UNiagaraComponent;
class USphereComponent;

UCLASS()
class PROJECTBLEED_API APBProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APBProjectile();

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	USphereComponent* SphereComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UProjectileMovementComponent* ProjectileMovementComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UNiagaraComponent* NiagaraComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UPBBulletHitReactionComponent* BulletHitReactionComponent = nullptr;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Fire Projectile in forward direction
	UFUNCTION()
	void FireProjectile();

	//Fire Projectile in specific direction
	UFUNCTION()
	void FireProjectileInDirection(const FVector& ShootDirection);

	//Get Projectile Movement Component
	UFUNCTION()
	UProjectileMovementComponent* GetProjectileMovementComponent() const { return ProjectileMovementComponent; }


};
