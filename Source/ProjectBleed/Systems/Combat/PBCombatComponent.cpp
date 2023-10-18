// All Rights belong to Studio WASD 2023

#include "PBCombatComponent.h"
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


void UPBCombatComponent::GiveWeapon(TSubclassOf<APBWeaponBase> WeaponClass)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = PBCharacterOwner;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (WeaponClass == nullptr)
	{
		V_LOG_ERROR(LogPBWeapon, TEXT("Invalid Weapon To Give Class"));
		return;
	}

	APBWeaponBase* SpawnedPBWeapon = Cast<APBWeaponBase>(GetWorld()->SpawnActor<APBWeaponBase>(WeaponClass, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation(), SpawnParams));
	if (!IsValid(SpawnedPBWeapon))
	{
		V_LOG_ERROR(LogPBWeapon, TEXT("Invalid SpawnedPBWeapon"));
		return;
	}

	CurrentWeapon = SpawnedPBWeapon;

	const FAttachmentTransformRules& AttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false);

	SpawnedPBWeapon->AttachToComponent(PBCharacterOwner->GetMesh(), AttachmentRules, FName(TEXT("weapon_r")));
	SpawnedPBWeapon->OnEquip();
}

void UPBCombatComponent::FireCurrentWeapon()
{
	if (CurrentWeapon == nullptr)
	{
		V_LOG_ERROR(LogPBWeapon, TEXT("Invalid CurrentWeapon"));
		return;
	}

	CurrentWeapon->OnFire();
}

void UPBCombatComponent::StopFireCurrentWeapon()
{
	if (CurrentWeapon == nullptr)
	{
		V_LOG_ERROR(LogPBWeapon, TEXT("Invalid CurrentWeapon"));
		return;
	}

	CurrentWeapon->OnUnEquip();
}

// Called every frame
void UPBCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

