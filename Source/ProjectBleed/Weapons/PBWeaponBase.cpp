// All Rights belong to Studio WASD 2023


#include "PBWeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ProjectBleed/Core/PBHUD.h"
#include "ProjectBleed/Libraries/CustomLogging.h"
#include "ProjectBleed/Player/PBPlayerController.h"
#include "ProjectBleed/Systems/Audio/PBAudioDetectionSubsystem.h"
#include "ProjectBleed/UI/PBWeaponWidget.h"
#include "ProjectBleed/Weapons/Components/PBBulletHitReactionComponent.h"
#include "ProjectBleed/Weapons/Components/PBThrowableWeaponComponent.h"
#include "ProjectBleed/Weapons/PBProjectile.h"

DEFINE_LOG_CATEGORY(LogPBWeapon)

// Sets default values
APBWeaponBase::APBWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
	WeaponMesh->SetCollisionProfileName(TEXT("Weapon"));

	ThrowableWeaponomponent = CreateDefaultSubobject<UPBThrowableWeaponComponent>(TEXT("ThrowableWeaponomponent"));
	ThrowableWeaponomponent->SetUpdatedComponent(WeaponMesh);

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

	//Only setup starting ammo to max ammo if no override has been provided.
	if (!bOverrideDefaultAmmoCount)
	{
		CurrentAmmo = WeaponData->MaxAmmo;
	}

	V_LOGF(LogPBWeapon, TEXT("Current Ammo is:"), GetCurrentAmmo());

	//Setup Fire Rate if using dynamic fire rate
	if (WeaponData->bUseDynamicFireRate)
	{
		UPBAudioDetectionSubsystem* AudioDetectionSubsystem = GetWorld()->GetSubsystem<UPBAudioDetectionSubsystem>();
		if (AudioDetectionSubsystem == nullptr)
		{
			V_LOG_ERROR(LogPBWeapon, TEXT("Invalid AudioDetectionSubsystem"));
			return;
		}
		WeaponData->FireRate = AudioDetectionSubsystem->GetBeatInterval(WeaponData->DynamicFireRateMode);
	}
}

void APBWeaponBase::Equip()
{	
	if (!ensureAlwaysMsgf(WeaponData, TEXT("Invalid WeaponData")))
	{
		return;
	}

	if (!ensureAlwaysMsgf(WeaponData->AnimationLayer, TEXT("Invalid AnimationLayer")))
	{
		return;
	}

	if (!ensureAlwaysMsgf(WeaponData->EquipAnimation, TEXT("Invalid EquipAnimation")))
	{		
		return;
	}

	if (ensureAlwaysMsgf(WeaponData->WeaponUIData, TEXT("Invalid Weapon UI Data")))
	{

		const APlayerController* PC = Cast<APlayerController>(GetInstigatorController());
		if (ensureMsgf(PC, TEXT("Invalid Player Controller")))
		{
			APBHUD* PBHUD = Cast<APBHUD>(PC->GetHUD());
			if (!ensureMsgf(PBHUD, TEXT("Invalid PBHUD")))
			{
				return;
			}
			PBHUD->UpdateWeaponWidget(this);
		}
	}

	GetPBOwner()->GetMesh()->LinkAnimClassLayers(WeaponData->AnimationLayer);
	GetPBOwner()->PlayAnimMontage(WeaponData->EquipAnimation);
}

void APBWeaponBase::UnEquip()
{
	if (WeaponData->AnimationLayer == nullptr)
	{
		V_LOG_ERROR(LogPBWeapon, TEXT("Invalid AnimationLayer"));
		return;
	}

	const APlayerController* PC = Cast<APlayerController>(GetInstigatorController());
	if (ensureMsgf(PC, TEXT("Invalid Player Controller")))
	{
		APBHUD* PBHUD = Cast<APBHUD>(PC->GetHUD());
		if (!ensureMsgf(PBHUD, TEXT("Invalid PBHUD")))
		{
			return;
		}
		PBHUD->RemoveWeaponWidget();
	}

	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

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
			// Check if it's time to start a new burst
			if (CurrentBurstCount <= 0)
			{
				CurrentBurstCount = WeaponData->BurstCount; // Set the number of shots in the burst
				GetWorld()->GetTimerManager().SetTimer(BurstFireTimerHandle, this, &APBWeaponBase::InternalBurstFire, WeaponData->FireRate, true, 0.f);
			}

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
	if (CurrentAmmo <= 0 || CurrentBurstCount <= 0)
	{
		StopFire();
		// Trigger InternalBurstFire to start a new burst
		return;
	}

	CurrentBurstCount--;
	InternalFire();
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
	SpawnBulletProjectile(HitResult);

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
	
	OutHitResult = HitResult;
	return bLineTraceResult;
}


void APBWeaponBase::StopFire()
{
	if (FireTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
	}
	if (BurstFireTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(BurstFireTimerHandle);
	}

	CurrentBurstCount = 0;
}

void APBWeaponBase::OverrideDefaultCurrentAmmoCount(const int InCurrentAmmo)
{
	bOverrideDefaultAmmoCount = true;
	SetCurrentAmmo(InCurrentAmmo);
}

UPBWeaponDataBase* APBWeaponBase::GetWeaponData() const
{
	if (!ensureAlwaysMsgf(WeaponData, TEXT("Weapon Data is Invalid")))
	{
		return nullptr;
	}

	return WeaponData;
}

APBCharacter* APBWeaponBase::GetPBOwner()
{
	if (PBOwnerCharacter == nullptr)
	{
		if (!ensureAlwaysMsgf(GetOwner(), TEXT("No Owner")))
		{
			return nullptr;
		}
		PBOwnerCharacter = Cast<APBCharacter>(GetOwner());
		return PBOwnerCharacter;
	}
	
	return PBOwnerCharacter;
}

void APBWeaponBase::ReduceAmmo(int AmmoToReduce)
{
	CurrentAmmo = FMath::Clamp(CurrentAmmo - AmmoToReduce, 0, WeaponData->MaxAmmo);
	OnBulletCountChange.Broadcast(CurrentAmmo);
}

void APBWeaponBase::AddAmmo(int AmmoToAdd)
{
	CurrentAmmo = FMath::Clamp(CurrentAmmo += AmmoToAdd, 0, WeaponData->MaxAmmo);
	OnBulletCountChange.Broadcast(CurrentAmmo);
}

void APBWeaponBase::SpawnBulletProjectile(const FHitResult& InHitResult)
{
	if (!ensureAlwaysMsgf(WeaponData, TEXT("Invalid WeaponData")))
	{
		return;
	}
	if (!ensureAlwaysMsgf(WeaponData->ProjectileData, TEXT("Invalid ProjectileData")))
	{
		return;
	}

	const FRotator& ShootDirection = UKismetMathLibrary::FindLookAtRotation(InHitResult.TraceStart, InHitResult.ImpactPoint);
	const FVector& StartLocation = InHitResult.TraceStart + (ShootDirection.Vector() * 100.f);

	const FTransform& SpawnTransform = FTransform(ShootDirection, StartLocation);
	APBProjectile* SpawnedProjectile = GetWorld()->SpawnActorDeferred<APBProjectile>(GetWeaponData()->ProjectileData->ProjectileClass.Get(), SpawnTransform, this, PBOwnerCharacter, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	if (!ensureAlwaysMsgf(SpawnedProjectile, TEXT("Invalid spawned PBProjectile")))
	{
		return;
	}

	SpawnedProjectile->SetProjectileData(GetWeaponData()->ProjectileData);
	UGameplayStatics::FinishSpawningActor(SpawnedProjectile, SpawnTransform);
}
	

// Called every frame
void APBWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
