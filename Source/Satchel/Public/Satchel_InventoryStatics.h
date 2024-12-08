// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Satchel_InventoryStatics.generated.h"


class USatchel_EquipmentComponent;
struct FGameplayTag;
DEFINE_LOG_CATEGORY_STATIC(LogInventoryStatics, Log, All)

class USatchel_ItemSlot;
class USatchel_ItemBase;
class ISatchel_Inventory;
/**
 * 
 */
UCLASS(BlueprintType)
class SATCHEL_API USatchel_InventoryStatics : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	/// 
	/// @param Inventory 
	/// @param Count 
	/// @param Item 
	/// @param SlotIndex Slot in inventory to add item to. Defaults to -1, which will find the first available slot
	/// @return 
	UFUNCTION(BlueprintCallable, Category="Satchel|Statics")
	static bool GiveInventoryItem(
		const TScriptInterface<ISatchel_Inventory> Inventory,
		const int Count,
		const USatchel_ItemBase* Item,
		const int SlotIndex = -1);

	UFUNCTION(BlueprintCallable, Category="Satchel|Statics")
	static bool DropInventoryItemByIndex(
		const TScriptInterface<ISatchel_Inventory> Inventory,
		const int Count,
		const int Index);

	/// Drops a specific item on the floor from the inventory.
	/// This will remove the specified item from the inventory.
	///
	/// This is not performant. For performance critical usages, use DropInventoryItemByIndex
	/// @param Inventory Inventory to remove item from
	/// @param Count How many of the item to remove. -1 is all. If there are not enough of the item, the drop will fail.
	/// @param Item The item to look for in the inventory. 
	/// @return 
	UFUNCTION(BlueprintCallable, Category="Satchel|Statics")
	static bool DropInventorySpecificItem(
		const TScriptInterface<ISatchel_Inventory> Inventory,
		const int Count,
		const USatchel_ItemBase* Item);

	/// Swaps the items from a slot on inventory A and a slot in inventory B
	/// ASlot <==> BSlot
	/// If either slot/index is null/invalid (Meaning you want to move an item from AInventory to BInventory, rather than swap)
	/// it will attempt to find the best valid slot in the inventory with the invalid slot index.
	///
	/// The purpose of this function is to swap items between inventories, NOT drop/give items.
	/// For example, equipping an item (Assuming equipment is an inventory), or moving items in and out of storage.
	///
	/// Theoretically, this could also be used for trading.
	/// @param AInventory One of the inventories to swap items between
	/// @param BInventory One of the inventories to swap items between
	/// @param ASlotIndex The index of the slot to use for swapping AInventory, -1 for first available
	/// @param BSlotIndex The index of the slot to use for swapping BInventory, -1 for first available
	/// @return 
	UFUNCTION(BlueprintCallable, Category="Satchel|Statics")
	static bool SwapInventoryItems(
		const TScriptInterface<ISatchel_Inventory> AInventory,
		const TScriptInterface<ISatchel_Inventory> BInventory,
		const int ASlotIndex,
		const int BSlotIndex);


	UFUNCTION(BlueprintCallable, Category="Satchel|Statics")
	static AActor* GetInventoryOwningActor(const TScriptInterface<ISatchel_Inventory> Inventory);

	UFUNCTION(BlueprintCallable, Category="Satchel|Statics")
	static TScriptInterface<ISatchel_Inventory> GetInventoryForActor(AActor* InventoryOwner);

	UFUNCTION(BlueprintCallable, Category="Satchel|Statics")
	static USatchel_EquipmentComponent* GetEquipmentForActor(AActor* EquipmentOwner);
	
	/// 
	/// @param ItemID 
	/// @param Inventory 
	/// @param RemoveFromInventory 
	/// @return 
	UFUNCTION(BlueprintCallable, Category="Satchel|Statics")
	static const USatchel_ItemBase* GetItemFromInventory(
		const FGameplayTag& ItemID,
		TScriptInterface<ISatchel_Inventory> Inventory,
		bool RemoveFromInventory = false);

private:
	
	
};
