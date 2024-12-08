// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Satchel_InventorySlotItemWidget.h"

#include "Components/Image.h"
#include "Core/Components/Satchel_EquipmentComponent.h"
#include "Core/Items/Satchel_ItemBase.h"
#include "Core/Items/Consumable/Satchel_ConsumableBase.h"
#include "Core/Items/Equipment/Satchel_EquipmentBase.h"
#include "Core/Items/Fragments/Satchel_ItemFragment_Equippable.h"
#include "Core/Items/Resource/Satchel_ResourceBase.h"
#include "UI/Satchel_InventorySlotWidget.h"

TSet<UContext_ActionEntry*> USatchel_InventorySlotItemWidget::GetActionEntries_Implementation() const {

	TSet<UContext_ActionEntry*> Entries;
	Entries.Append(ContainedItem->ContextEntries);
	Entries.Append(Super::GetActionEntries_Implementation());
	return Entries;
}

void USatchel_InventorySlotItemWidget::SetItem(const USatchel_ItemBase* Item) {
	ContainedItem = Item;
	
	if (IsValid(ContainedItem)) {
		
		ensure(ContainedItem->Icon);
		Icon->SetBrushFromTexture(Item->Icon);

		ClearTags();
		
		// Equipment check
		USatchel_ItemFragment_Equippable* EquipFragment = Item->FindFragment<USatchel_ItemFragment_Equippable>();
		if (IsValid(EquipFragment)) {
			OnEquipmentItemAdded(Item, EquipFragment);
			return;
		}

		// Consumable
		const USatchel_ConsumableBase* Consumable = Cast<USatchel_ConsumableBase>(Item);
		if (IsValid(Consumable)) {
			OnConsumableItemAdded(Consumable);
			return;
		}

		// Resource
		const USatchel_ResourceBase* Resource = Cast<USatchel_ResourceBase>(Item);
		if (IsValid(Resource)) {
			OnResourceItemAdded(Resource);
			return;
		}

		// Other
		OnOtherItemAdded(Item);
		
	}
}

const USatchel_ItemBase* USatchel_InventorySlotItemWidget::GetItem() const {
	return ContainedItem;
}

USatchel_InventorySlotWidget* USatchel_InventorySlotItemWidget::GetOwner() const {
	return Owner;
}

void USatchel_InventorySlotItemWidget::SetOwner(USatchel_InventorySlotWidget* NewOwner) {
	Owner = NewOwner;
}

