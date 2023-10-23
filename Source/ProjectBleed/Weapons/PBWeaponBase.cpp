// All Rights belong to Studio WASD 2023


#include "PBWeaponBase.h"
#include <ProjectBleed/Libraries/CustomLogging.h>

DEFINE_LOG_CATEGORY(LogPBWeapon)

// Sets default values
APBWeaponBase::APBWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
}

// Called when the game starts or when spawned
void APBWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	PBOwnerCharacter = Cast<APBCharacter>(GetOwner());
	if (!IsValid(PBOwnerCharacter))
	{
		UE_LOG(LogPBWeapon, Error, TEXT("Invalid owner"));
	}	
}

void APBWeaponBase::Equip()
{	
	if (AnimationLayer == nullptr)
	{
		V_LOG_ERROR(LogPBWeapon, TEXT("Invalid AnimationLayer"));
		return;
	}

	if (EquipAnimation == nullptr)
	{
		V_LOG_ERROR(LogPBWeapon, TEXT("Invalid EquipAnimation"));
		return;
	}

	PBOwnerCharacter->GetMesh()->LinkAnimClassLayers(AnimationLayer);
	PBOwnerCharacter->PlayAnimMontage(EquipAnimation);
}

void APBWeaponBase::UnEquip()
{
	if (AnimationLayer == nullptr)
	{
		V_LOG_ERROR(LogPBWeapon, TEXT("Invalid AnimationLayer"));
		return;
	}

	PBOwnerCharacter->GetMesh()->UnlinkAnimClassLayers(AnimationLayer);
}

void APBWeaponBase::Fire()
{
}

void APBWeaponBase::StopFire()
{
}

// Called every frame
void APBWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
