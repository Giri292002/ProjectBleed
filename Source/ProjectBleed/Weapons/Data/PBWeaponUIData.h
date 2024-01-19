// All Rights belong to Studio WASD 2023

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/Texture2D.h"
#include "PBWeaponUIData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBLEED_API UPBWeaponUIData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "ProjectBleed | Defaults")
	UTexture2D* WeaponIcon;	
};
