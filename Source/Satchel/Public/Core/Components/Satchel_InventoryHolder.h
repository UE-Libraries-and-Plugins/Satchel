// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Satchel_InventoryHolder.generated.h"

class USatchel_InventoryComponent;
// This class does not need to be modified.
UINTERFACE(BlueprintType, Blueprintable)
class SATCHEL_API USatchel_InventoryHolder : public UInterface {
	GENERATED_BODY()
};

/**
 * Defines an object that should contain an inventory, and allows fetching said inventory, or replacing it 
 */
class SATCHEL_API ISatchel_InventoryHolder {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory|Holder")
	USatchel_InventoryComponent* GetInventory() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory|Holder")
	void GiveInventory(USatchel_InventoryComponent* NewInventory);
};
