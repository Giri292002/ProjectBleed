// All Rights belong to Studio WASD 2023


#include "PBHealthComponent.h"
#include "PBHealthData.h"

DEFINE_LOG_CATEGORY(LogPBHealthComponent);

// Sets default values for this component's properties
UPBHealthComponent::UPBHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPBHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!ensureAlwaysMsgf(HealthData, TEXT("Invalid Health Data. Did you attach a data to this component?")))
	{
		return;
	}

	CurrentHealth = CurrentMaxHealth = HealthData->MaxHealth;
	
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UPBHealthComponent::OnTakeAnyDamage);
}

void UPBHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogPBHealthComponent, Log, TEXT("Damaged Actor: %s for %f by %s controlled by %s"), *(GetNameSafe(DamagedActor)), Damage, *(GetNameSafe(DamageCauser)), *(GetNameSafe(InstigatedBy)));

	if (Damage <= 0)
	{
		return;
	}

	CurrentHealth -= Damage;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0, CurrentMaxHealth);

	UE_LOG(LogPBHealthComponent, Log, TEXT("Health of %s : %i"), *GetNameSafe(GetOwner()), GetCurrentHealth());
}


// Called every frame
void UPBHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

