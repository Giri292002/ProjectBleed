// All Rights belong to Studio WASD 2023


#include "PBThrowableWeaponComponent.h"
#include "ProjectBleed/Weapons/PBWeaponBase.h"
#include "ProjectBleed/Weapons/Pickups/PBWeaponPickupBase.h"
#include "ProjectBleed/Libraries/CustomLogging.h"

void UPBThrowableWeaponComponent::OnStopped(const FHitResult& Imapct)
{
	APBWeaponBase* WeaponOwner = Cast<APBWeaponBase>(GetOwner());
	if (!ensureMsgf(WeaponOwner, TEXT("No Weapon Owner")))
	{
		return;
	}
	
	if (WeaponOwner->GetCurrentAmmo() <= 0)
	{
		V_LOG(LogPBThrowableComponent, TEXT("No Ammo, no need to spawn pickup"));
		WeaponOwner->Destroy();
		return;
	}

	TSubclassOf<APBWeaponPickupBase> WeaponPickupToSpawn = WeaponOwner->GetWeaponData()->WeaponPickupToSpawn;
	
	if (!ensureAlwaysMsgf(WeaponPickupToSpawn, TEXT("No Weapon Pickup Spawn class defined in weapon data")))
	{
		return;
	}
	
	APBWeaponPickupBase* SpawnedPickup = GetWorld()->SpawnActorDeferred<APBWeaponPickupBase>(WeaponPickupToSpawn.Get(), WeaponOwner->GetTransform());
	if (!ensureAlwaysMsgf(SpawnedPickup, TEXT("Could not spawn pickup")))
	{
		return;
	}
	
	SpawnedPickup->OverrideDefaultCurrentAmmoCount(WeaponOwner->GetCurrentAmmo());

	SpawnedPickup->FinishSpawning(WeaponOwner->GetTransform());

	WeaponOwner->Destroy();
}
