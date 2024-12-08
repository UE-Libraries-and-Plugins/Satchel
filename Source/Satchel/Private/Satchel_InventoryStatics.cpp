// Fill out your copyright notice in the Description page of Project Settings.

#include "Satchel_InventoryStatics.h"

#include "Core/Satchel_ItemInstance.h"
#include "Core/Satchel_Statics.h"
#include "Core/Components/Satchel_EquipmentComponent.h"
#include "Core/Components/Satchel_EquipmentHolder.h"
#include "Core/Components/Satchel_Inventory.h"
#include "Core/Components/Satchel_InventoryComponent.h"
#include "Core/Components/Satchel_InventoryHolder.h"
#include "GameFramework/PlayerState.h"
#include "UI/Satchel_InventoryUIStatics.h"

bool USatchel_InventoryStatics::GiveInventoryItem(
	const TScriptInterface<ISatchel_Inventory> Inventory,
	const int Count,
	const USatchel_ItemBase* Item,
	const int SlotIndex) {

	UObject* InventoryObj = Inventory.GetObject();

	if (!ISatchel_Inventory::Execute_CanAcceptItem(InventoryObj, Item, SlotIndex)) {
		UE_LOG(LogInventoryStatics, Display, TEXT("Attempted to give item to %s, but cannot accept item"), *InventoryObj->GetName())
		return false;
	}
	
	ISatchel_Inventory::Execute_AddItem(InventoryObj, Item, SlotIndex);

	const AActor* InventoryOwner = GetInventoryOwningActor(Inventory);
	if (IsValid(InventoryOwner)) {
		USatchel_InventoryUIStatics::SpawnItemInventoryPopup(
			{Item->Icon, Item->DisplayName, false},
			InventoryOwner);
	} else {
		UE_LOG(LogInventoryStatics, Warning, TEXT("Item granted to inventory, but unable to spawn UI! Inventory does not have valid actor"))
	}
	
	return true;
}

bool USatchel_InventoryStatics::DropInventoryItemByIndex(
	const TScriptInterface<ISatchel_Inventory> Inventory,
	const int Count,
	const int Index) {

	UObject* InventoryObj = Inventory.GetObject();

	const AActor* InventoryWorldActor = GetInventoryOwningActor(Inventory);
	if (!IsValid(InventoryWorldActor)) {
		UE_LOG(LogInventoryStatics, Warning,
			TEXT("Attempted to drop an item, but the inventory %s does not have a valid actor type"),
			*InventoryObj->GetName())
		return false;
	}
	
	// fail fast validations
	if (!ISatchel_Inventory::Execute_CanRemoveItem(InventoryObj, Index)) {
		UE_LOG(LogInventoryStatics, Display,
			TEXT("Attempted to drop an item, but the inventory %s can not remove item"),
			*InventoryObj->GetName())
		return false;
	}

	// Rm item and drop it
	const USatchel_ItemBase* Item = ISatchel_Inventory::Execute_RemoveItem(InventoryObj, Index);
	// Cant spawn null item
	if (Item == nullptr) {
		UE_LOG(LogInventoryStatics, Display,
			TEXT("Attempted to drop an item, but the item removed was null"))
		return false;
	}

	const ASatchel_ItemInstance* Obj =  USatchel_Statics::SpawnWorldItem(
		InventoryObj,
		Item->GetClass(),
		InventoryWorldActor->GetActorLocation(),
		InventoryWorldActor->GetActorRotation());

	return IsValid(Obj);
}


bool USatchel_InventoryStatics::DropInventorySpecificItem(
	const TScriptInterface<ISatchel_Inventory> Inventory,
	const int Count,
	const USatchel_ItemBase* Item) {

	UObject* InventoryObj = Inventory.GetObject();

	const int Index = ISatchel_Inventory::Execute_FindSlotIndexForExistingItem(InventoryObj, Item->ItemId);
	if (Index == -1) {
		return false;
	}

	return DropInventoryItemByIndex(Inventory, Count, Index);
}

bool USatchel_InventoryStatics::SwapInventoryItems(const TScriptInterface<ISatchel_Inventory> AInventory,
                                                   const TScriptInterface<ISatchel_Inventory> BInventory,
                                                   const int ASlotIndex,
                                                   const int BSlotIndex) {
	
	// The first thing we need to do is ask each inventory
	// Hey, can you accept this item in this slot?
	// If both say yes, we can do the swap
	// If either says no, we nope out
	UObject* AInventoryObj = AInventory.GetObject();
	UObject* BInventoryObj = BInventory.GetObject();

	// Fetch slots so we can see whats in them
	const USatchel_ItemSlot* ASlot = ISatchel_Inventory::Execute_GetInventorySlotAtIndex(AInventoryObj, ASlotIndex);
	const USatchel_ItemSlot* BSlot = ISatchel_Inventory::Execute_GetInventorySlotAtIndex(BInventoryObj, BSlotIndex);

	// B Valid, A is not
	if (!IsValid(ASlot) && IsValid(BSlot)) {
		ASlot = ISatchel_Inventory::Execute_GetBestSlotForItem(AInventoryObj, BSlot->PeekItem());
	}

	// A is valid, b is not
	if (IsValid(ASlot) && !IsValid(BSlot)) {
		BSlot = ISatchel_Inventory::Execute_GetBestSlotForItem(BInventoryObj, ASlot->PeekItem());
	}
	
	if (!IsValid(ASlot) || !IsValid(BSlot)) {
		return false;
	}
 	
	// ok if null
	const USatchel_ItemBase* AItem = ASlot->PeekItem();
	const USatchel_ItemBase* BItem = BSlot->PeekItem();

	// Check if can accept item, get slot from slot item rather than index in case provided index is -1 (First available)
	const bool ACanAccept = ISatchel_Inventory::Execute_CanAcceptItem(AInventoryObj, BItem, ASlot->GetSlotIndex());
	const bool BCanAccept = ISatchel_Inventory::Execute_CanAcceptItem(BInventoryObj, AItem, BSlot->GetSlotIndex());
	
	if (!ACanAccept || !BCanAccept) {
		return false;
	}

	// We don't need to check result because we already know items can be accepted
	// if null is passed, item is expected to be removed
	ISatchel_Inventory::Execute_AddItem(AInventoryObj, BItem, ASlot->GetSlotIndex());
	ISatchel_Inventory::Execute_AddItem(BInventoryObj, AItem, BSlot->GetSlotIndex());

	
	return true;
}

AActor* USatchel_InventoryStatics::GetInventoryOwningActor(const TScriptInterface<ISatchel_Inventory> Inventory) {

	UObject* InventoryObj = Inventory.GetObject();

	// The actor has a component which implements the interface - Most likely situation
	if (const UActorComponent* InventoryComp = Cast<UActorComponent>(InventoryObj); IsValid(InventoryComp)) {

		// If component, then the owner may be a player state (This would usually be true for players) 
		const APlayerState* PlayerState = Cast<APlayerState>(InventoryComp->GetOwner());
		if (IsValid(PlayerState)) {
			return PlayerState->GetPawn();
		}

		return InventoryComp->GetOwner();
	}

	// The actor is implementing the inventory interface directly (Not great, but possible)
	if(AActor* InventoryActor = Cast<AActor>(InventoryObj); IsValid(InventoryActor)) {
		return InventoryActor;
	}
	
	return nullptr;
}

TScriptInterface<ISatchel_Inventory> USatchel_InventoryStatics::GetInventoryForActor(AActor* InventoryOwner) {
	
	if(InventoryOwner->Implements<USatchel_InventoryHolder>()) {
		return ISatchel_InventoryHolder::Execute_GetInventory(InventoryOwner);
	}

	// Actor has an inventory component
	UActorComponent* InventoryComp = InventoryOwner->GetComponentByClass(USatchel_InventoryComponent::StaticClass());
	if (IsValid(InventoryComp)) {
		return InventoryComp;
	}

	// If player state, try checks again on the state. 
	APlayerState* PlayerState = Cast<APlayerState>(InventoryOwner->GetOwner());
	if (IsValid(PlayerState)) {
		return GetInventoryForActor(PlayerState);
	}

	return nullptr;
}

USatchel_EquipmentComponent* USatchel_InventoryStatics::GetEquipmentForActor(AActor* EquipmentOwner) {

	if(EquipmentOwner->Implements<USatchel_EquipmentHolder>()) {
		return ISatchel_EquipmentHolder::Execute_GetEquipment(EquipmentOwner);
	}
	
	// Actor has an inventory component
	USatchel_EquipmentComponent* InventoryComp = Cast<USatchel_EquipmentComponent>(EquipmentOwner->GetComponentByClass(USatchel_EquipmentComponent::StaticClass()));
	if (IsValid(InventoryComp)) {
		return InventoryComp;
	}

	// If player state, try checks again on the state. 
	APlayerState* PlayerState = Cast<APlayerState>(EquipmentOwner->GetOwner());
	if (IsValid(PlayerState)) {
		return GetEquipmentForActor(PlayerState);
	}

	return nullptr;
}

const USatchel_ItemBase* USatchel_InventoryStatics::GetItemFromInventory(
	const FGameplayTag& ItemID,
	TScriptInterface<ISatchel_Inventory> Inventory,
	const bool RemoveFromInventory) {

	// Will match closest tag found
	UObject* InventoryObj = Inventory.GetObject();
	const int Index = Inventory->Execute_FindSlotIndexForExistingItem(InventoryObj, ItemID);
	if(Index == -1) return nullptr;

	// We want to TAKE the item from the inventory, this will remove it entirely and return it
	if (RemoveFromInventory) {
		if (Inventory->Execute_CanRemoveItem(InventoryObj, Index)) {
			return Inventory->Execute_RemoveItem(InventoryObj, Index);
		}
	}

	// We don't want to remove the item, we just want to peek it
	const USatchel_ItemSlot* ItemSlot = Inventory->Execute_GetInventorySlotAtIndex(InventoryObj, Index);
	if (!IsValid(ItemSlot)) return nullptr;

	return ItemSlot->PeekItem();
}
