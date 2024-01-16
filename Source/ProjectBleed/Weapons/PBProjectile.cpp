// All Rights belong to Studio WASD 2023


#include "PBProjectile.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "NiagaraComponent.h"
#include "ProjectBleed/Libraries/CustomLogging.h"
#include "ProjectBleed/Weapons/Data/PBProjectileData.h"
#include "ProjectBleed/Weapons/Components/PBBulletHitReactionComponent.h"

// Sets default values
APBProjectile::APBProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComponent);
	SphereComponent->SetCollisionProfileName("Projectile");
	SphereComponent->SetSphereRadius(5.0f);
	SphereComponent->SetNotifyRigidBodyCollision(true);
	SphereComponent->OnComponentHit.AddDynamic(this, &APBProjectile::OnComponentHit);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(SphereComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

	ProjectileMovementComponent->SetUpdatedComponent(SphereComponent);
	ProjectileMovementComponent->InitialSpeed = 3000.0f;
	ProjectileMovementComponent->MaxSpeed = 0.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	BulletHitReactionComponent = CreateDefaultSubobject<UPBBulletHitReactionComponent>(TEXT("BulletHitReactionComponent"));
}

void APBProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Log, TEXT("Projectile Hit: %s"), *OtherActor->GetName());
	
	//TODO: Do other types of damage based on an enum in ProjectileData

	const FPointDamageEvent PointDamageEvent = FPointDamageEvent();
	OtherActor->TakeDamage(GetProjectileData()->WeaponDamage, PointDamageEvent, GetInstigatorController(), GetInstigator());

	BulletHitReactionComponent->PlayImpactFX(Hit);
	Destroy();
}

// Called when the game starts or when spawned
void APBProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetProjectileMovementComponent()->InitialSpeed = GetProjectileData()->ProjectileSpeed;
}

// Called every frame
void APBProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APBProjectile::FireProjectile()
{
	FireProjectileInDirection(GetActorForwardVector());
}

void APBProjectile::FireProjectileInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

UPBProjectileData* APBProjectile::GetProjectileData() const
{
	if (ensureAlwaysMsgf(ProjectileData, TEXT("ProjectileData is Invalid")))
	{
		return ProjectileData;
	}
	return nullptr;
}

void APBProjectile::SetProjectileData(UPBProjectileData* InProjectileData)
{
	if (ensureAlwaysMsgf(InProjectileData, TEXT("InProjectileData is invalid")))
	{
		ProjectileData = InProjectileData;
	}
}

