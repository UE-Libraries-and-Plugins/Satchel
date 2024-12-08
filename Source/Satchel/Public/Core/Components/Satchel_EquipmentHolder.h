// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Satchel_EquipmentHolder.generated.h"

class USatchel_EquipmentComponent;
// This class does not need to be modified.
UINTERFACE()
class USatchel_EquipmentHolder : public UInterface {
	GENERATED_BODY()
};

/**
 * 
 */
class SATCHEL_API ISatchel_EquipmentHolder {
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory|Holder")
	USatchel_EquipmentComponent* GetEquipment() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory|Holder")
	void GiveEquipment(USatchel_EquipmentComponent* NewInventory);
};
