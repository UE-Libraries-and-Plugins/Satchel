// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Satchel_InventoryWidget.h"

#include "LilHelperLite.h"
#include "Components/UniformGridPanel.h"
#include "Core/Components/Satchel_InventoryComponent.h"
#include "UI/Satchel_InventorySlotWidget.h"

void USatchel_InventoryWidget::GetGiverContextEntries_Implementation(TSet<UContext_ActionEntry*>& OutContextEntries) {
	OutContextEntries.Append(ContextEntries);
}

void USatchel_InventoryWidget::RegenerateSlots(USatchel_InventoryComponent* LinkedInventory, int32 SetSlotSize, int32 SetColumns) {

	const uint32 UsedSlotSize = SetSlotSize == 0 ? SlotSize : SetSlotSize;
	const uint32 UsedColumnsCount = SetColumns == 0 ? Columns : SetColumns;
	
	if (!IsValid(LinkedInventory)) return;
	InventoryGrid->ClearChildren();

	// Generate each slot and assigns it to a position on the grid
	for (int i = 0; i < ISatchel_Inventory::Execute_GetInventorySize(LinkedInventory); ++i) {
		int32 Row;
		int32 Col;

		FLilHelper::IndexToGridPosition(i, UsedColumnsCount, Row, Col);
		
		USatchel_InventorySlotWidget* SlotWidget = CreateWidget<USatchel_InventorySlotWidget>(this, InventorySlotTemplate);
		ensure(SlotWidget);

		SlotWidget->SetInventorySlot(ISatchel_Inventory::Execute_GetInventorySlotAtIndex(LinkedInventory, i));
		SlotWidget->PrepareSlot();
		
		InventoryGrid->AddChildToUniformGrid(SlotWidget, Row, Col);
	}

	SetAllSlotsToSize(UsedSlotSize);
}

void USatchel_InventoryWidget::SetAllSlotsToSize(int32 SetSlotSize) {
	for (const auto ChildSlot : InventoryGrid->GetAllChildren()) {
		if (USatchel_InventorySlotWidget* InventorySlot = Cast<USatchel_InventorySlotWidget>(ChildSlot); IsValid(InventorySlot)) {
			InventorySlot->SetSize(SetSlotSize, SetSlotSize);
		}
	}
}
