// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Satchel_EquipmentSlotEnum.h"
#include "Core/Items/Satchel_ItemBase.h"
#include "Satchel_EquipmentBase.generated.h"

struct FAttributeMetaData;
struct FGameplayAbilitySpecHandle;
struct FActiveGameplayEffectHandle;


UENUM()
enum EC_EquipmentType {
	Armor,
	WeaponGear,
	ToolGear
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SATCHEL_API USatchel_EquipmentBase : public USatchel_ItemBase {
	GENERATED_BODY()
	
};
