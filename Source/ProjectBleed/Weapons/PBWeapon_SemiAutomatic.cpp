#include "PBWeapon_SemiAutomatic.h"

// Sets default values
APBWeapon_SemiAutomatic::APBWeapon_SemiAutomatic()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void APBWeapon_SemiAutomatic::Fire()
{
	Super::Fire();
}
