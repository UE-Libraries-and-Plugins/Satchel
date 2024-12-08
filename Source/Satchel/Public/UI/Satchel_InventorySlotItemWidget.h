// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Components/Satchel_InventoryComponent.h"
#include "UI/Context_UIWidgetBase.h"
#include "Satchel_InventorySlotItemWidget.generated.h"

class USatchel_ConsumableBase;
class USatchel_ResourceBase;
class USatchel_EquipmentBase;
class USatchel_InventorySlotWidget;
class USatchel_ItemSlot;
class UImage;

/**
 * 
 */
UCLASS()
class SATCHEL_API USatchel_InventorySlotItemWidget : public UContext_UIWidgetBase {
	GENERATED_BODY()

	UPROPERTY()
	const USatchel_ItemBase* ContainedItem;

	UPROPERTY()
	USatchel_InventorySlotWidget* Owner;
	
public:

	virtual TSet<UContext_ActionEntry*> GetActionEntries_Implementation() const override;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> Icon;

	UFUNCTION(BlueprintCallable)
	void SetItem(const USatchel_ItemBase* Item);

	UFUNCTION(BlueprintCallable)
	const USatchel_ItemBase* GetItem() const;

	UFUNCTION(BlueprintCallable)
	USatchel_InventorySlotWidget* GetOwner() const;

	UFUNCTION(BlueprintCallable)
	void SetOwner(USatchel_InventorySlotWidget* NewOwner);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnEquipmentItemAdded(const USatchel_ItemBase* Equipment, USatchel_ItemFragment_Equippable* EquipFragment);

	UFUNCTION(BlueprintImplementableEvent)
	void OnResourceItemAdded(const USatchel_ResourceBase* Resource);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnConsumableItemAdded(const USatchel_ConsumableBase* Consumable);

	UFUNCTION(BlueprintImplementableEvent)
	void OnOtherItemAdded(const USatchel_ItemBase* Item);
};
