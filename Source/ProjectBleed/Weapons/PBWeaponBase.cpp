// All Rights belong to Studio WASD 2023


#include "PBWeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include <ProjectBleed/Libraries/CustomLogging.h>

DEFINE_LOG_CATEGORY(LogPBWeapon)

// Sets default values
APBWeaponBase::APBWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetRootComponent());

	UAnimInstance* AnimInstance = LoadObject<UAnimInstance>(nullptr, TEXT("/Game/ProjectBleed/Animations/Weapons/ABP_Weapon_Base"));
	if (AnimInstance)
	{
		WeaponMesh->SetAnimInstanceClass(AnimInstance->GetClass());
	}
}

// Called when the game starts or when spawned
void APBWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponData == nullptr)
	{
		V_LOG_ERROR(LogPBWeapon, TEXT("Invalid WeaponData"));
		return;
	}

	PBOwnerCharacter = Cast<APBCharacter>(GetOwner());
	if (!IsValid(PBOwnerCharacter))
	{
		UE_LOG(LogPBWeapon, Error, TEXT("Invalid owner"));
	}	
}

void APBWeaponBase::InternalFire()
{
	if (WeaponMesh->GetAnimInstance() && WeaponData->WeaponFireAnimation)
	{
		WeaponMesh->GetAnimInstance()->Montage_Play(WeaponData->WeaponFireAnimation);
	}
	else
	{
		V_LOG(LogPBWeapon, TEXT("Invalid WeaponFireAnimation"));
	}
	if (WeaponData->CharacterFireAnimation)
	{
		PBOwnerCharacter->PlayAnimMontage(WeaponData->CharacterFireAnimation);
	}
	else
	{
		V_LOG(LogPBWeapon, TEXT("Invalid CharacterFireAnimation"));
	}
}

void APBWeaponBase::InternalBurstFire()
{	
	if (CurrentBurstCount + 1 >= WeaponData->BurstCount)
	{
		StopFire();
		return;
	}

	CurrentBurstCount++;
	InternalFire();
	GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &APBWeaponBase::InternalBurstFire, WeaponData->FireRate, false, 0.f);	
}

void APBWeaponBase::Equip()
{	
	if (WeaponData == nullptr)
	{
		V_LOG_ERROR(LogPBWeapon, TEXT("Invalid WeaponData"));
		return;
	}

	if (WeaponData->AnimationLayer == nullptr)
	{
		V_LOG_ERROR(LogPBWeapon, TEXT("Invalid AnimationLayer"));
		return;
	}

	if (WeaponData->EquipAnimation == nullptr)
	{
		V_LOG(LogPBWeapon, TEXT("Invalid EquipAnimation"));
	}

	PBOwnerCharacter->GetMesh()->LinkAnimClassLayers(WeaponData->AnimationLayer);
	PBOwnerCharacter->PlayAnimMontage(WeaponData->EquipAnimation);
}

void APBWeaponBase::UnEquip()
{
	if (WeaponData->AnimationLayer == nullptr)
	{
		V_LOG_ERROR(LogPBWeapon, TEXT("Invalid AnimationLayer"));
		return;
	}

	PBOwnerCharacter->GetMesh()->UnlinkAnimClassLayers(WeaponData->AnimationLayer);
}

void APBWeaponBase::Fire()
{
	switch (WeaponData->WeaponFireMode)
	{
		case EFireMode::SemiAuto:
		{
			InternalFire();
			break;
		}
		case EFireMode::FullAuto:
		{
			GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &APBWeaponBase::InternalFire, WeaponData->FireRate, true, 0.f);
			break;
		}
		case EFireMode::Burst:
		{
			InternalBurstFire();
			break;
		}
		default:
		{
			V_LOG(LogPBWeapon, TEXT("Invalid WeaponFireMode"));
			break;
		}
	}

	UPBScoringSubsystem* PBScoringSystem = GetWorld()->GetSubsystem<UPBScoringSubsystem>();

	if(WeaponData->ScoreData != nullptr && PBScoringSystem != nullptr)
	{
		PBScoringSystem->AddToScore(WeaponData->ScoreData);
	}
	else
	{
		V_LOG(LogPBWeapon, TEXT("Invalid ScoreData or PBScoringSystem"));
	}
}

void APBWeaponBase::StopFire()
{
	if (FireTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
	}

	CurrentBurstCount = 0;
}

// Called every frame
void APBWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
