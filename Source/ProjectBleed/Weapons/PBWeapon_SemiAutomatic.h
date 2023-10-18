#pragma once

#include "CoreMinimal.h"
#include "PBWeaponBase.h"
#include "PBWeapon_SemiAutomatic.generated.h"

UCLASS()
class PROJECTBLEED_API APBWeapon_SemiAutomatic : public APBWeaponBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	APBWeapon_SemiAutomatic();

	virtual void OnFire() override;
};
