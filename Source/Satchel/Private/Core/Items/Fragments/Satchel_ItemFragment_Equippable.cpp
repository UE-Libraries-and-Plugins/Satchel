// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Items/Fragments/Satchel_ItemFragment_Equippable.h"

#include "Core/Components/Satchel_EquipmentComponent.h"

void USatchel_ItemFragment_Equippable::Equip(USatchel_EquipmentComponent* EquipmentInventory) {
	if (!IsValid(EquipmentInventory)) {
		UE_LOG(LogCharacterEquipment, Warning, TEXT("Attempting to equip an item to an invalid inventory!"))
		return;
	}

	bIsEquipped = true;
	OwningInventory = EquipmentInventory;
	OwningActor = OwningInventory->GetOwner();
}

void USatchel_ItemFragment_Equippable::UnEquip() {
	bIsEquipped = false;
	OwningInventory = nullptr;
	OwningActor = nullptr;
}
