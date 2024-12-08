// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Satchel_ItemFragment_Base.h"
#include "Satchel_ItemFragment_Requirements.generated.h"

/**
 * 
 */
UCLASS()
class SATCHEL_API USatchel_ItemFragment_Requirements : public USatchel_ItemFragment_Base {
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "Requirements", meta=(AllowPrivateAccess))
	TMap<FGameplayAttribute, float>  UsageRequirements;
};
