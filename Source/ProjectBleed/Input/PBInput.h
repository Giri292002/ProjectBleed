// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PBInput.generated.h"

class UInputMappingContext;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPBInput : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTBLEED_API IPBInput
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	UInputMappingContext* GetInputMappingContext();
};
