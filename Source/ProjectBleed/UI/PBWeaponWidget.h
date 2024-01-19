// All Rights belong to Studio WASD 2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PBWeaponWidget.generated.h"

/**
 * 
 */
class UImage;
class UTextBlock;
class APBWeaponBase;
class UWidgetAnimation;
class UPBWeaponUIData;

UCLASS()
class PROJECTBLEED_API UPBWeaponWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/*
	* Update the current widget's info
	* @param InWeaponOwner The current weapon that will own this widget
	* @param
	*/
	UFUNCTION(BlueprintCallable)
	void UpdateWeaponData(APBWeaponBase* InWeaponOwner, UPBWeaponUIData* const InWeaponData);

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* WeaponImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentAmmoText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* BulletCountChangeAnimation;

	UPROPERTY()
	APBWeaponBase* CurrentWeaponOwner;

	UPROPERTY()
	UPBWeaponUIData* CurrentWeaponData;

	UFUNCTION()
	void OnBulletCountChange(int32 CurrentAmmo);

	void NativePreConstruct() override;
	void NativeConstruct() override;
};
