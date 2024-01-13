// All Rights belong to Studio WASD 2023

#pragma once

#include "CoreMinimal.h"


UENUM(BlueprintType)
enum class EFireMode : uint8
{
	SemiAuto,   //Fires once per click
	FullAuto,   //Fires while the mouse is held down
	Burst       //Fires a set amount of bullets per click
};

UENUM()
enum class EBeatType : uint8
{
	Whole = 0,
	Half = 1,
	Quarter = 2,
	Any = 3
};

UENUM()
enum class EBeatSpawnDirection : uint8
{
	Left = 0,
	Right = 1
};