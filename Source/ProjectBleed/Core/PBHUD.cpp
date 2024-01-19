// Fill out your copyright notice in the Description page of Project Settings.


#include "PBHUD.h"
#include "Blueprint/UserWidgetBlueprint.h"
#include "ProjectBleed/UI/PBCrosshair.h"
#include "ProjectBleed/UI/PBWeaponWidget.h"
#include "ProjectBleed/Weapons/PBWeaponBase.h"
#include "ProjectBleed/Weapons/Data/PBWeaponUIData.h"


void APBHUD::BeginPlay()
{
	Super::BeginPlay();

	if (ensureAlwaysMsgf(CrosshairWidgetClass, TEXT("Crosshair Widget is invalid")))
	{
		//Only create crosshair if we dont already have one
		if (CrosshairWidget == nullptr)
		{
			CrosshairWidget = CreateWidget<UPBCrosshair>(GetWorld(), CrosshairWidgetClass);
			CrosshairWidget->AddToViewport();
		}
	}
}

UPBWeaponWidget* APBHUD::CreateWeaponWidget(APBWeaponBase* const InWeaponOwner)
{
	if (!ensureAlwaysMsgf(WeaponWidgetClass, TEXT("WeaponWidget is null")))
	{
		return nullptr;
	}

	if (!ensureAlwaysMsgf(InWeaponOwner, TEXT("InWeaponOwner is null")))
	{
		return nullptr;
	}

	//Dont need to add another weapon widget. Only need one instance
	if (GetCurrentWeaponWidget())
	{
		return nullptr;
	}

	CurrentWeaponWidget = CreateWidget<UPBWeaponWidget>(GetWorld(), WeaponWidgetClass);	

	if (ensureAlwaysMsgf(CurrentWeaponWidget, TEXT("Invalid Current Weapon Widget")))
	{
		return CurrentWeaponWidget;
	}

	return nullptr;
}

void APBHUD::UpdateWeaponWidget(APBWeaponBase* const InWeaponOwner)
{
	if (GetCurrentWeaponWidget() == nullptr)
	{
		CreateWeaponWidget(InWeaponOwner);
	}

	if (!ensureAlwaysMsgf(InWeaponOwner, TEXT("InWeaponOwner is invalid")))
	{
		return;
	}

	if (!ensureAlwaysMsgf(InWeaponOwner->GetWeaponData()->WeaponUIData, TEXT("Weapon UI Data is invalid")))
	{
		return;
	}

	GetCurrentWeaponWidget()->UpdateWeaponData(InWeaponOwner, InWeaponOwner->GetWeaponData()->WeaponUIData);
	GetCurrentWeaponWidget()->AddToViewport();
}

void APBHUD::RemoveWeaponWidget()
{
	//Weapon widget is already null
	if (GetCurrentWeaponWidget() == nullptr)
	{
		return;
	}

	GetCurrentWeaponWidget()->RemoveFromViewport();
}

UPBWeaponWidget* APBHUD::GetCurrentWeaponWidget()
{
	return CurrentWeaponWidget;
}