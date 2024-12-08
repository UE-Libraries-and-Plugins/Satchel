// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Satchel_InventoryModifierBase.generated.h"

class USatchel_ItemBase;
class ISatchel_Inventory;
/**
 * 
 */
UCLASS(Abstract)
class SATCHEL_API USatchel_InventoryModifierBase : public UObject {
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void OnCreated(TScriptInterface<ISatchel_Inventory> Inventory) const {}

	UFUNCTION()
	virtual bool CanAccept(TScriptInterface<ISatchel_Inventory> Inventory, USatchel_ItemBase* Item) const { return true; }

	UFUNCTION()
	virtual bool CanRemove(TScriptInterface<ISatchel_Inventory> Inventory, USatchel_ItemBase* Item) const { return true; }
	
	UFUNCTION()
	virtual void OnItemAccept(TScriptInterface<ISatchel_Inventory> Inventory, USatchel_ItemBase* Item) const {}

	UFUNCTION()
	virtual void OnItemRemoved(TScriptInterface<ISatchel_Inventory> Inventory, USatchel_ItemBase* Item) const {}
};
