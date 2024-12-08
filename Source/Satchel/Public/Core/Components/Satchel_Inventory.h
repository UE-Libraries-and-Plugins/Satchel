// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Items/Satchel_ItemBase.h"
#include "UObject/Interface.h"
#include "Satchel_Inventory.generated.h"

class ISatchel_Inventory;
class USatchel_ItemBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemChanged, const USatchel_ItemBase*,  OldItem, const USatchel_ItemBase*,  NewItem);

/*
 * Represents a singular slot within an inventory or container that handles storing an item
 */
UCLASS(DefaultToInstanced, EditInlineNew)
class SATCHEL_API USatchel_ItemSlot : public UObject {
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<USatchel_ItemBase> MustBeType = nullptr;
	
	UPROPERTY(EditAnywhere)
	const USatchel_ItemBase* SlottedItem = nullptr;

	UPROPERTY()
	int SlotIndex = -1;

	UPROPERTY()
	UObject* OwningInventoryPtr;
	
public:

	/// The type of item that this slot can contain - This acts as a restriction
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer AllowedItems;

	// If the item can be removed/added
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsLocked = false;
	
	FOnItemChanged OnItemChanged;

public:
	/**
	 * Constrains the inventory slot to a specified type
	 * @param Constraint 
	 */
	UFUNCTION(BlueprintCallable)
	void ConstrainToType(TSubclassOf<USatchel_ItemBase> Constraint);

	UFUNCTION(BlueprintCallable)
	virtual bool CanAcceptItem(const USatchel_ItemBase* Item) const;
	
	/**
	 * Sets the item held by the slot - This will replace the existing item and return it.
	 * Use the inventory implementation instead of directly modifying the slot, unless you're trying to bypass inventory functionality
	 * @param NewItem The item to add to the slot
	 * @param OutOldItem The existing item, if any
	 * @return If setting the item was successful
	 */
	UFUNCTION(BlueprintCallable)
	const USatchel_ItemBase* SetItem(const USatchel_ItemBase* NewItem);

	/**
	 * Retrieve an item from the slot - Removes the item entirely
	 * Use the inventory implementation instead of directly modifying the slot, unless you're trying to bypass inventory functionality
	 * @return 
	 */
	UFUNCTION(BlueprintCallable)
	const USatchel_ItemBase* TakeItem();

	/**
	 * Views the item in the slot without removing it from the slot
	 * @return 
	 */
	UFUNCTION(BlueprintCallable)
	const USatchel_ItemBase* PeekItem() const;

	/**
	 * Get the index in which this slot belongs within its inventory
	 * @return 
	 */
	UFUNCTION(BlueprintCallable)
	int GetSlotIndex() const;

	/**
	 * Gets and returns the owning inventory of this slot 
	 */
	UFUNCTION(BlueprintCallable)
	TScriptInterface<ISatchel_Inventory> GetInventory() const;
	
	/**
	 * Setup the inventory and index within that inventory - Required to run on init
	 */
	UFUNCTION(BlueprintCallable)
	void Setup(int Index, UObject* Inventory, FGameplayTagContainer& TagContainer);
};


// This class does not need to be modified.
UINTERFACE(BlueprintType, Blueprintable)
class SATCHEL_API USatchel_Inventory : public UInterface {
	GENERATED_BODY()
};

/**
 * Interface denoting a basic inventory type (Ability to get slots, for example)
 */
class SATCHEL_API ISatchel_Inventory {
	GENERATED_BODY()

public:
	/** Retrieve the max size of the inventory */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory|Size")
	int32 GetInventorySize() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory|Slots")
	TArray<USatchel_ItemSlot*> GetAllSlotsOfType(const FGameplayTag& ItemTag);
	
	/** Gets an inventory slot allocated to an index. The index is very important, because its the slots position
	 * in the inventory, as well as its position on the UI */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory|Slots")
	USatchel_ItemSlot* GetInventorySlotAtIndex(int32 Index);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory|Slots")
	USatchel_ItemSlot* GetBestSlotForItem(const USatchel_ItemBase* Item);
	
	/** Removes item from every slot in inventory, and passes it to the caller */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,  Category = "Inventory|Slots")
	TArray<USatchel_ItemBase*> ClearAllSlots();

	/** Finds the slot where this item exists, if any.
		@return Slot index, or -1 if item does not exist in inventory 
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory|Slots")
	int FindSlotIndexForExistingItem(const FGameplayTag& ItemToFind);
	
	/**
	 * 
	 * @param NewItem 
	 * @param Index Index where you want to set the item. -1 To use the first available slot
	 * @return 
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory|Items")
	bool CanAcceptItem(const USatchel_ItemBase* NewItem, int Index = -1);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory|Items")
	bool CanRemoveItem(int Index);
	
	/**
	 * Removed the item at the desired slot
	 * @return The item that was removed
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory|Items")
	const USatchel_ItemBase* RemoveItem(int Index);

	/**
	 * Search through inventory until you find an item - Remove that item
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory|Items")
	const USatchel_ItemBase* RemoveItemUnknownIndex(const FGameplayTag& ObjectToRemove, int Count);
	
	/**
	 * Adds an item at the first available slot, or specified slot, and returns the existing item (if any) 
	 * @param index The index of the slot where you want to add the item. -1 for the first available slot
	 * @param NewItem The new item to add
	 * @return The existing item in the slot
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory|Items")
	const USatchel_ItemBase* AddItem(const USatchel_ItemBase* NewItem, int Index = -1);
};
