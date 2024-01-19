// All Rights belong to Studio WASD 2023


#include "PBWeaponWidget.h"
#include "Animation/WidgetAnimation.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "ProjectBleed/Weapons/PBWeaponBase.h"
#include "ProjectBleed/Weapons/Data/PBWeaponUIData.h"


void UPBWeaponWidget::NativePreConstruct()
{

}

void UPBWeaponWidget::NativeConstruct()
{
	if (!ensureAlwaysMsgf(CurrentWeaponOwner, TEXT("Invalid Weapon Owner. Make sure to call UpdateWeaponData() to set weapon owner when constructing this widget")))
	{
		return;
	}

	if (!ensureAlwaysMsgf(CurrentWeaponData, TEXT("Invalid Weapon Data. Make  sure to Call UpdateWeaponData() to set weapon data when constructing this widget")))
	{
		return;
	}
	CurrentWeaponOwner->OnBulletCountChange.AddDynamic(this, &UPBWeaponWidget::OnBulletCountChange);

	//Default to current weapon ammo
	CurrentAmmoText->SetText(FText::FromString(FString::FromInt(CurrentWeaponOwner->GetCurrentAmmo())));
}

void UPBWeaponWidget::UpdateWeaponData(APBWeaponBase* InWeaponOwner, UPBWeaponUIData* const InWeaponData)
{

	if (ensureAlwaysMsgf(InWeaponOwner, TEXT("Invalid InWeaponOwner")))
	{
		CurrentWeaponOwner = InWeaponOwner;
	}

	if (ensureAlwaysMsgf(InWeaponData, TEXT("Invalid InWeaponData")))
	{
		CurrentWeaponData = InWeaponData;
	}

	if (ensureMsgf(InWeaponData->WeaponIcon, TEXT("Weapon Icon is null")))
	{
		WeaponImage->SetBrushFromTexture(InWeaponData->WeaponIcon);
	}
}

void UPBWeaponWidget::OnBulletCountChange(int32 CurrentAmmo)
{
	CurrentAmmoText->SetText(FText::FromString(FString::FromInt(CurrentAmmo)));
	PlayAnimation(BulletCountChangeAnimation);	
}