// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Satchel_LootDefinition.generated.h"

class USatchel_ItemBase;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FSatchel_LootDefinition : public FTableRowBase{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item" )
	TSubclassOf<USatchel_ItemBase>  ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int Weight = 0;
};
