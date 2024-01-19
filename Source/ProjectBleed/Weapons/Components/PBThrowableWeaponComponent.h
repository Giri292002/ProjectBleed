// All Rights belong to Studio WASD 2023

#pragma once

#include "CoreMinimal.h"
#include "ProjectBleed/Systems/PBThrowableObjectComponent.h"
#include "PBThrowableWeaponComponent.generated.h"

/**
 * Same functionality as a throwable object but also spawns a weapon pickup with the correct ammo configuration based on the thrown weapon
 */
UCLASS()
class PROJECTBLEED_API UPBThrowableWeaponComponent : public UPBThrowableObjectComponent
{
	GENERATED_BODY()
	
protected:
	virtual void OnStopped(const FHitResult& Imapct) override;
};
