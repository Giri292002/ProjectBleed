// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MaterialLibraries.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBLEED_API UMaterialLibraries : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Material",meta=(Keywords="Static, Switch, Bool, Material, Parameter, Instance"))
	static void SetMaterialInstanceStaticSwitchParameterValue(UMaterialInstance* Instance, FName ParameterName, bool Value);
};
