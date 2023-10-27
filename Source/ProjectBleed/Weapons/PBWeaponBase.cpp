// All Rights belong to Studio WASD 2023


#include "PBWeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "ProjectBleed/Player/PBPlayerController.h"
#include "ProjectBleed/Weapons/Components/PBBulletHitReactionComponent.h"
#include <ProjectBleed/Libraries/CustomLogging.h>
#include <Kismet/KismetSystemLibrary.h>

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

	BulletHitReactionComponent = CreateDefaultSubobject<UPBBulletHitReactionComponent>(TEXT("BulletHitReactionComponent"));

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
		return;
	}	

	PBPlayerController = Cast<APBPlayerController>(PBOwnerCharacter->GetController());
	if (!IsValid(PBPlayerController))
	{
		UE_LOG(LogPBWeapon, Error, TEXT("Invalid Player Controller"));
		return;
	}

	CurrentAmmo = WeaponData->MaxAmmo;
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
	if (HasAmmo() == false)
	{
		OnNoAmmo();
		return;
	}

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

	//Add Score

	UPBScoringSubsystem* PBScoringSystem = GetWorld()->GetSubsystem<UPBScoringSubsystem>();

	if (WeaponData->ScoreData != nullptr && PBScoringSystem != nullptr)
	{
		PBScoringSystem->AddToScore(WeaponData->ScoreData);
	}
	else
	{
		V_LOG(LogPBWeapon, TEXT("Invalid ScoreData or PBScoringSystem"));
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

void APBWeaponBase::InternalFire()
{
	if (CurrentAmmo <= 0)
	{
		OnNoAmmo();
		return;
	}

	ReduceAmmo();

	FHitResult HitResult;
	PerformLineTrace(HitResult);

	BulletHitReactionComponent->PlayImpactFX(HitResult);

	VSCREENMSGF("Current Ammo: %i", CurrentAmmo);

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

void APBWeaponBase::OnNoAmmo()
{
	StopFire();

	if (WeaponData->EmptyMagAnimation)
	{
		PBOwnerCharacter->PlayAnimMontage(WeaponData->EmptyMagAnimation);
	}
	else
	{
		V_LOG(LogPBWeapon, TEXT("Invalid EmptyMagAnimation"));
	}
}

bool APBWeaponBase::PerformLineTrace(FHitResult& OutHitResult)
{
	FHitResult HitResult;
	const FVector& StartLocation = WeaponMesh->GetSocketLocation(TEXT("Muzzle"));
	FVector EndLocation = StartLocation + PBPlayerController->GetMouseWorldDirection(StartLocation) * WeaponData->WeaponRange;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	CollisionQueryParams.AddIgnoredActor(PBOwnerCharacter);
	CollisionQueryParams.bReturnPhysicalMaterial = true;

	//Perform Line Trace
	bool bLineTraceResult = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility, CollisionQueryParams);
	//DrawDebugLine(GetWorld(), StartLocation, EndLocation, HitResult.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 10.0f);
	OutHitResult = HitResult;
	return bLineTraceResult;
}


void APBWeaponBase::StopFire()
{
	if (FireTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
	}

	CurrentBurstCount = 0;
}

void APBWeaponBase::ReduceAmmo(int AmmoToReduce)
{
	CurrentAmmo -= AmmoToReduce;
	CurrentAmmo = FMath::Clamp(CurrentAmmo, 0, WeaponData->MaxAmmo);
}

void APBWeaponBase::AddAmmo(int AmmoToAdd)
{
	CurrentAmmo += AmmoToAdd;
	CurrentAmmo = FMath::Clamp(CurrentAmmo, 0, WeaponData->MaxAmmo);
}

// Called every frame
void APBWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
