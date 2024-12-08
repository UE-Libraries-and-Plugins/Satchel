// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Satchel_ItemFragment_Base.h"
#include "UObject/Object.h"
#include "Satchel_ItemFragment_Power.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SATCHEL_API USatchel_ItemFragment_Power : public USatchel_ItemFragment_Base {
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Power = 404;
};
