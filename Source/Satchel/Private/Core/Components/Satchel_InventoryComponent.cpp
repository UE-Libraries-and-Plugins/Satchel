// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/Components/Satchel_InventoryComponent.h"

#include "Core/Items/Satchel_ItemBase.h"

// Sets default values for this component's properties
USatchel_InventoryComponent::USatchel_InventoryComponent() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	InventorySlots.SetNum(InventoryMaxSize);
}

USatchel_ItemSlot* USatchel_InventoryComponent::GetInventorySlotAtIndex_Implementation(const int32 Index) {
	// should be between -1 and the max size of the inventory
	if (Index > InventoryMaxSize || Index < -1) return nullptr;

	int32 UsedIndex = Index;

	// If -1 is provided, we need to find the first available slot
	if (Index == -1) {
		for (const auto Slot : InventorySlots) {
			if (IsValid(Slot)) {
				if (Slot->PeekItem() == nullptr) {
					UsedIndex = Slot->GetSlotIndex();
					break;
				}
			}
		}
		if(UsedIndex == -1) {
			return nullptr;
		}
	}
	
	return InventorySlots[UsedIndex];
}

USatchel_ItemSlot* USatchel_InventoryComponent::GetBestSlotForItem_Implementation(const USatchel_ItemBase* Item) {
	return Execute_GetInventorySlotAtIndex(this, -1);
}

int USatchel_InventoryComponent::FindSlotIndexForExistingItem_Implementation(const FGameplayTag& ItemToFind) {
	for (const auto Slot : InventorySlots) {
		if (IsValid(Slot)) {
			const USatchel_ItemBase* Item = Slot->PeekItem();
			
			if (IsValid(Item) && Item->ItemId.MatchesTag(ItemToFind)) {
				return Slot->GetSlotIndex();
			}
		}
	}
	return -1;
}

TArray<USatchel_ItemSlot*> USatchel_InventoryComponent::GetAllSlotsOfType_Implementation(const FGameplayTag& ItemTag) {
	TArray<USatchel_ItemSlot*> Slots;
	
	for (const auto Slot : InventorySlots) {
		for (const auto AllowedItem : Slot->AllowedItems) {
			if (AllowedItem.MatchesTag(ItemTag)) {
				Slots.Add(Slot);
			}	
		}
	}
	return Slots;
}


void USatchel_InventoryComponent::ConstrainAllSlots(TSubclassOf<USatchel_ItemBase> Constraint) {
	for (USatchel_ItemSlot* Slot : InventorySlots) {
		Slot->ConstrainToType(Constraint);
	}
}

TArray<USatchel_ItemBase*> USatchel_InventoryComponent::ClearAllSlots_Implementation() {
	TArray<USatchel_ItemBase*> ExistingItems;

	for(USatchel_ItemSlot* Slot : InventorySlots) {
		if (IsValid(Slot->PeekItem())) {
			ExistingItems.Add(const_cast<USatchel_ItemBase*>(Slot->TakeItem()));
		}
	}

	return ExistingItems;
}

// Called when the game starts
void USatchel_InventoryComponent::BeginPlay() {
	Super::BeginPlay();

	for (int i = 0; i < InventoryMaxSize; ++i) {
		USatchel_ItemSlot* Slot = NewObject<USatchel_ItemSlot>();
		Slot->Setup(i, this, AllowedItemsTag);
		InventorySlots[i] = Slot;
	}

	// Assign starter items that were set in the blueprint
	for (int i = 0; i < StarterItems.Num(); ++i) {
		if (i < InventoryMaxSize) {
			USatchel_ItemSlot* Slot = Execute_GetInventorySlotAtIndex(this, i);

			USatchel_ItemBase* Item = NewObject<USatchel_ItemBase>(this, StarterItems[i]); 
			// We know there's no item yet
			Slot->SetItem(Item);
		}
	}
	
}

bool USatchel_InventoryComponent::CanRemoveItem_Implementation(int Index) {
	const USatchel_ItemSlot* SlotAtIndex = Execute_GetInventorySlotAtIndex(this, Index);
	
	return IsValid(SlotAtIndex) && !SlotAtIndex->bIsLocked;
}

const USatchel_ItemBase* USatchel_InventoryComponent::RemoveItem_Implementation(int Index) {
	USatchel_ItemSlot* DesiredSlot = Execute_GetInventorySlotAtIndex(this, Index);
	if (!IsValid(DesiredSlot)) return nullptr;

	const USatchel_ItemBase* ItemTaken =  DesiredSlot->TakeItem();
	return ItemTaken;
}

bool USatchel_InventoryComponent::CanAcceptItem_Implementation(const USatchel_ItemBase* NewItem, int Index) {
	const USatchel_ItemSlot* SlotAtIndex = Execute_GetInventorySlotAtIndex(this, Index);
	return (
		IsValid(SlotAtIndex) &&
		!SlotAtIndex->bIsLocked &&
		SlotAtIndex->CanAcceptItem(NewItem));
}


const USatchel_ItemBase* USatchel_InventoryComponent::AddItem_Implementation(const USatchel_ItemBase* NewItem, int Index) {
	USatchel_ItemSlot* DesiredSlot = Execute_GetInventorySlotAtIndex(this, Index);
	if (!IsValid(DesiredSlot) || !Execute_CanAcceptItem(this, NewItem, Index)) return nullptr;

	return DesiredSlot->SetItem(NewItem);
}

// Does not use count yet 
const USatchel_ItemBase* USatchel_InventoryComponent::RemoveItemUnknownIndex_Implementation(
	const FGameplayTag& ObjectToRemove,
	int Count) {
	for (const auto Slot : InventorySlots) {
		if (Slot->PeekItem()->ItemId.MatchesTagExact(ObjectToRemove)) {
			if (!Execute_CanRemoveItem(this, Slot->GetSlotIndex())) return nullptr;
			const USatchel_ItemBase* Item = Slot->TakeItem();
			return Item;
		}
	}
	return nullptr;
}

