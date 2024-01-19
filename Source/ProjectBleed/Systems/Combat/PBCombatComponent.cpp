// All Rights belong to Studio WASD 2023

#include "PBCombatComponent.h"
#include "ProjectBleed/Weapons/Components/PBThrowableWeaponComponent.h"
#include "ProjectBleed/Libraries/CustomLogging.h"

// Sets default values for this component's properties
UPBCombatComponent::UPBCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPBCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	PBCharacterOwner = Cast<APBCharacter>(GetOwner());	

	if (DefaultWeapon != nullptr)
	{
		GiveWeapon(DefaultWeapon);
	}	
	else
	{
		V_LOG_ERROR(LogPBWeapon, TEXT("Invalid DefaultWeapon"));
	}
}


void UPBCombatComponent::GiveWeapon(TSubclassOf<APBWeaponBase> WeaponClass, bool bOverrideDefaultAmmoCount, int InCurrentAmmo)
{
	//Check if we already have a weapon and remove it
	if (CurrentWeapon != nullptr)
	{
		RemoveWeapon();
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = PBCharacterOwner;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (WeaponClass == nullptr)
	{
		V_LOG_ERROR(LogPBWeapon, TEXT("Invalid Weapon To Give Class"));
		return;
	}

	APBWeaponBase* SpawnedPBWeapon = Cast<APBWeaponBase>(GetWorld()->SpawnActorDeferred<APBWeaponBase>(WeaponClass, GetOwner()->GetActorTransform(), PBCharacterOwner, PBCharacterOwner, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (!ensureAlwaysMsgf(SpawnedPBWeapon, TEXT("Invalid SpawnedPBWeapon")))
	{
		return;
	}

	if (bOverrideDefaultAmmoCount)
	{
		SpawnedPBWeapon->OverrideDefaultCurrentAmmoCount(InCurrentAmmo);
	}

	SpawnedPBWeapon->FinishSpawning(GetOwner()->GetActorTransform());
	CurrentWeapon = SpawnedPBWeapon;

	const FAttachmentTransformRules& AttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);

	SpawnedPBWeapon->AttachToComponent(PBCharacterOwner->GetMesh(), AttachmentRules, FName(TEXT("weapon_r_socket")));
	SpawnedPBWeapon->Equip();
}

void UPBCombatComponent::RemoveWeapon()
{
	if (CurrentWeapon == nullptr)
	{
		V_LOG_ERROR(LogPBWeapon, TEXT("Invalid CurrentWeapon"));
		return;
	}

	CurrentWeapon->UnEquip();
	CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	CurrentWeapon = nullptr;
}

void UPBCombatComponent::FireCurrentWeapon()
{
	if (CurrentWeapon == nullptr)
	{
		V_LOG_ERROR(LogPBWeapon, TEXT("Invalid CurrentWeapon"));
		return;
	}

	CurrentWeapon->Fire();
}

void UPBCombatComponent::StopFireCurrentWeapon()
{
	if (CurrentWeapon == nullptr)
	{
		V_LOG_ERROR(LogPBWeapon, TEXT("Invalid CurrentWeapon"));
		return;
	}

	CurrentWeapon->StopFire();
}

void UPBCombatComponent::ThrowWeapon()
{
	APBWeaponBase* WeaponInHand = GetCurrentWeapon();
	RemoveWeapon();
	WeaponInHand->GetComponentByClass<UPBThrowableWeaponComponent>()->Throw();
}

// Called every frame
void UPBCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

