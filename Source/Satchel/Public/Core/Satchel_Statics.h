// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Satchel_Statics.generated.h"

class ASatchel_ItemInstance;
class USatchel_ItemBase;
/**
 * 
 */
UCLASS()
class SATCHEL_API USatchel_Statics : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="Satchel|Statics|Spawn")
	static ASatchel_ItemInstance* SpawnWorldItem(UObject* Context, TSubclassOf<USatchel_ItemBase> ItemToSpawn, FVector SpawnLocation, FRotator SpawnRotation);

	UFUNCTION(BlueprintCallable, Category="Satchel|Statics|Spawn")
	static ASatchel_ItemInstance* SpawnWorldItemFromLootTable(UObject* Context, const UDataTable* LootTableDataAsset, FVector Location, FRotator SpawnRotation);
};
