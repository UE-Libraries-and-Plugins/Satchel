// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Satchel_ToolTypeEnum.h"
#include "Core/Items/Equipment/Satchel_EquipmentBase.h"
#include "Satchel_ToolBase.generated.h"

/**
 * 
 */
UCLASS()
class SATCHEL_API USatchel_ToolBase : public USatchel_EquipmentBase {
	GENERATED_BODY()

	/// The type of tool that this is - This will be used to fetch, categorize, and interact with this item.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,  Category = "Tool", meta=(AllowPrivateAccess))
	TEnumAsByte<ESatchel_ToolTypeEnum> ToolType = WoodcuttingTool;
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tool")
	float ToolPower = 10.f;
	
};
