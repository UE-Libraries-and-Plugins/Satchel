// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Components/Satchel_Inventory.h"

#include "Core/Items/Satchel_ItemBase.h"
#include "Core/Components/Satchel_InventoryComponent.h"


// Item slot

void USatchel_ItemSlot::ConstrainToType(const TSubclassOf<USatchel_ItemBase> Constraint) {
	MustBeType = Constraint;
}

bool USatchel_ItemSlot::CanAcceptItem(const USatchel_ItemBase* Item) const {

	if (bIsLocked) return false;

	if (IsValid(Item)) {

		if (!Item->ItemId.MatchesAny(AllowedItems)) {
			return false;
		}

		if (!Item->FragmentsCanAccept(this)) {
			return false;
		}
		
		if (IsValid(MustBeType))
			return Item->IsA(MustBeType);
	}
	return true;
}

const USatchel_ItemBase* USatchel_ItemSlot::SetItem(const USatchel_ItemBase* NewItem) {
	// NewItem might be null, which is ok! An empty slot would be null. If not null, it has to be of specified type
	if (IsValid(NewItem) && IsValid(MustBeType)) {
		if (!NewItem->IsA(MustBeType)) {
			UE_LOG(LogInventory, Log, TEXT("Inventory slot is constrained to %p, but %p was provided"), *MustBeType, SlottedItem->GetClass());
			return nullptr;
		}	
	}
	
	if (bIsLocked) return nullptr;
	
	const USatchel_ItemBase* OldItem = SlottedItem;
	SlottedItem = NewItem;

	OnItemChanged.Broadcast(OldItem, SlottedItem);
	
	return OldItem;
}

const USatchel_ItemBase* USatchel_ItemSlot::TakeItem() {

	if (bIsLocked) return nullptr;
	
	const USatchel_ItemBase* TmpItem = SlottedItem;
	SlottedItem = nullptr;
	OnItemChanged.Broadcast(TmpItem, nullptr);
	return TmpItem;
}

const USatchel_ItemBase* USatchel_ItemSlot::PeekItem() const {
	return SlottedItem;
}

int USatchel_ItemSlot::GetSlotIndex() const {
	return SlotIndex;
}

TScriptInterface<ISatchel_Inventory> USatchel_ItemSlot::GetInventory() const {
	return OwningInventoryPtr;
}

void USatchel_ItemSlot::Setup(int Index, UObject* Inventory, FGameplayTagContainer& TagContainer) {
	SlotIndex = Index;
	OwningInventoryPtr = Inventory;
	AllowedItems = TagContainer;
}
