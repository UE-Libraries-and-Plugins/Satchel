// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Equipment/Satchel_EquipmentWidget.h"

#include "Components/PanelWidget.h"
#include "Core/Components/Satchel_EquipmentComponent.h"
#include "UI/Satchel_InventorySlotWidget.h"

void USatchel_EquipmentWidget::SetAllSlotsToSize(int32 SetSlotSize) {
	const int32 UsedSlotSize = SetSlotSize == 0 ? SlotSize : SetSlotSize;

	HeadSlot->SetSize(UsedSlotSize, UsedSlotSize);
	ChestSlot->SetSize(UsedSlotSize, UsedSlotSize);
	LegSlot->SetSize(UsedSlotSize, UsedSlotSize);
	WeaponSlot->SetSize(UsedSlotSize, UsedSlotSize);
	HandSlot->SetSize(UsedSlotSize, UsedSlotSize);
	BootSlot->SetSize(UsedSlotSize, UsedSlotSize);
	ToolASlot->SetSize(UsedSlotSize, UsedSlotSize);
}

void USatchel_EquipmentWidget::RegenerateSlots(USatchel_EquipmentComponent* LinkedInventory) {

	HeadSlot->SetInventorySlot(LinkedInventory->GetInventorySlotOfType(HeadSlotTag)); \
	HeadSlot->PrepareSlot();

	ChestSlot->SetInventorySlot(LinkedInventory->GetInventorySlotOfType(ChestSlotTag)); \
	ChestSlot->PrepareSlot();

	LegSlot->SetInventorySlot(LinkedInventory->GetInventorySlotOfType(LegSlotTag)); \
	LegSlot->PrepareSlot();

	WeaponSlot->SetInventorySlot(LinkedInventory->GetInventorySlotOfType(WeaponSlotTag)); \
	WeaponSlot->PrepareSlot();

	HandSlot->SetInventorySlot(LinkedInventory->GetInventorySlotOfType(HandSlotTag)); \
	HandSlot->PrepareSlot();

	BootSlot->SetInventorySlot(LinkedInventory->GetInventorySlotOfType(BootSlotTag)); \
	BootSlot->PrepareSlot();

	ToolASlot->SetInventorySlot(LinkedInventory->GetInventorySlotOfType(ToolASlotTag)); \
	ToolASlot->PrepareSlot();
	
	SetAllSlotsToSize(48);
}

void USatchel_EquipmentWidget::NativeOnInitialized() {
	Super::NativeOnInitialized();

}

void USatchel_EquipmentWidget::GetGiverContextEntries_Implementation(TSet<UContext_ActionEntry*>& OutContextEntries) {
	OutContextEntries.Append(ContextEntries);
}

