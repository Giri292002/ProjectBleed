// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PBHUD.generated.h"

/**
 * 
 */
class UPBWeaponWidget;
class UPBCrosshair;
class APBWeaponBase;

UCLASS()
class PROJECTBLEED_API APBHUD : public AHUD
{
	GENERATED_BODY()

public:

	/*
	* Updates the information on current weapon widget and add it to viewport. Adds a new weapon widget if there isn't one
	* @param InWeaponOwner The Weapon owner that will own this widget now
	* @param InWeaponUIData The UI Data of the current weapon
	*/
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateWeaponWidget(APBWeaponBase* const InWeaponOwner);

	//Hides weapon widget if there is one
	UFUNCTION(BlueprintCallable, Category = "UI")
	void RemoveWeaponWidget();

	/*
	* Gets current weapon widget if available
	* @return Current Weapon Widget
	*/
	UFUNCTION(BlueprintCallable, Category = "UI")
	UPBWeaponWidget* GetCurrentWeaponWidget();

protected:

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UPBCrosshair> CrosshairWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UPBWeaponWidget> WeaponWidgetClass;

	/*
	* Adds a weapon widget to the screen.
	* @param InWeaponWidget The Weapon Widget to Add
	*/
	UFUNCTION(Category = "UI")
	UPBWeaponWidget* CreateWeaponWidget(APBWeaponBase* const InWeaponOwner);

private:
	UUserWidget* CrosshairWidget;
	UPBWeaponWidget* CurrentWeaponWidget;

	virtual void BeginPlay() override;
};
