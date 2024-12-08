// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Satchel_InventorySlotWidget.h"
#include "Satchel.h"
#include "Satchel_InventoryStatics.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Core/Components/Satchel_EquipmentComponent.h"
#include "Core/Items/Satchel_ItemBase.h"
#include "Core/Components/Satchel_InventoryComponent.h"
#include "Operations/UMGC_ElementDragDropOperation.h"
#include "UI/Satchel_InventorySlotItemWidget.h"

void USatchel_InventorySlotWidget::GetGiverContextEntries_Implementation(TSet<UContext_ActionEntry*>& OutContextEntries) {
	OutContextEntries.Append(ContextEntries);
}

UContext_ActionEntry* USatchel_InventorySlotWidget::GetPrimaryContextEntry_Implementation() {
	return PrimaryContext;
}

void USatchel_InventorySlotWidget::SetSize(int32 Height, int32 Width) {
	if (!IsValid(SlotBorder)) return;

	UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(SlotBorder->Slot);
	if (!IsValid(PanelSlot)) return;

	SlotSize = FVector2D(Height, Width);
	
	PanelSlot->SetSize(SlotSize);
}

FVector2D USatchel_InventorySlotWidget::GetSize() const {
	return SlotSize;
}

void USatchel_InventorySlotWidget::SetInventorySlot(USatchel_ItemSlot* InventorySlot) {
	if (!IsValid(InventorySlot)) {
		UE_LOG(LogCharacterEquipment, Warning, TEXT("Equipment slot widget %s is linked to a nonexistent slot"),
			*GetName())
		return;
	}
	
	CachedInventorySlot = InventorySlot;
	
	InventorySlot->OnItemChanged.AddDynamic(this, &USatchel_InventorySlotWidget::Refresh);
}

const USatchel_ItemSlot* USatchel_InventorySlotWidget::GetInventorySlot() const {
	return CachedInventorySlot;
}

void USatchel_InventorySlotWidget::PrepareSlot() {
	if (!IsValid(CachedInventorySlot)) return;
	
	SlotItemWidget = CreateWidget<USatchel_InventorySlotItemWidget>(this, SlotItemWidgetTemplate);
	SlotItemWidget->SetOwner(this);
	SlotBorder->AddChild(SlotItemWidget);

	Refresh();
}

void USatchel_InventorySlotWidget::Refresh() {

	if (IsValid(SlotItemWidget)) {
		SlotItemWidget->SetItem(nullptr);
		SlotItemWidget->SetVisibility(ESlateVisibility::Hidden);

		if (const USatchel_ItemBase* Item = CachedInventorySlot->PeekItem(); IsValid(Item)) {
			SlotItemWidget->SetVisibility(ESlateVisibility::Visible);
			SlotItemWidget->SetItem(Item);
		}
		return;
	} 

	UE_LOG(LogSatchel, Warning, TEXT("Attempting to refresh inventory slot, but the item widget is null?"))
}

void USatchel_InventorySlotWidget::Refresh(const USatchel_ItemBase* Old, const USatchel_ItemBase* New) {
	Refresh();
}

bool USatchel_InventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                          UDragDropOperation* InOperation) {

	const UUMGC_ElementDragDropOperation* DragDropOp = Cast<UUMGC_ElementDragDropOperation>(InOperation);
	if (!IsValid(DragDropOp)) {
		return false;
	}

	const USatchel_InventorySlotItemWidget* FromSlotItem = Cast<USatchel_InventorySlotItemWidget>(DragDropOp->Payload);
	if(!IsValid(FromSlotItem)) {
		return false;
	}

	const int FromSlotIndex = FromSlotItem->GetOwner()->GetInventorySlot()->GetSlotIndex();
	const int ToSlotIndex = GetInventorySlot()->GetSlotIndex();

	TScriptInterface<ISatchel_Inventory> AInventory = GetInventorySlot()->GetInventory();

	const USatchel_ItemSlot* BSlot = FromSlotItem->GetOwner()->GetInventorySlot();
	TScriptInterface<ISatchel_Inventory> BInventory = BSlot->GetInventory();
	
	USatchel_InventoryStatics::SwapInventoryItems(
		AInventory,
		BInventory,
		ToSlotIndex,
		FromSlotIndex);
	
	return false;
}
