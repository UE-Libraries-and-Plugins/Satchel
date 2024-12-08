// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Satchel_SlottedInventory.h"
#include "Blueprint/UserWidget.h"
#include "Components/UniformGridPanel.h"
#include "Interface/Context_Giver.h"
#include "Satchel_InventoryWidget.generated.h"


class USatchel_InventoryComponent;
class USatchel_InventorySlotWidget;
/**
 * 
 */
UCLASS()
class SATCHEL_API USatchel_InventoryWidget : public UUserWidget, public ISatchel_SlottedInventory, public IContext_Giver {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> InventorySlotTemplate;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> InventoryGrid;

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	uint32 SlotSize = 64;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	uint32 Columns = 8;

	UPROPERTY(EditDefaultsOnly)
	TSet<UContext_ActionEntry*> ContextEntries;
	
public:

	virtual void GetGiverContextEntries_Implementation(TSet<UContext_ActionEntry*>& OutContextEntries) override;
	
	/**
	 * Regenerates the inventory UI for the specified inventory
	 * @param LinkedInventory The inventory to use when generating the UI
	 * @param SetSlotSize Size of each slot
	 * @param SetColumns Number of columns to generate (Fewer columns = more rows)
	 */
	UFUNCTION(BlueprintCallable)
	void RegenerateSlots(USatchel_InventoryComponent* LinkedInventory, int32 SetSlotSize = 0, int32 SetColumns = 0);

	UFUNCTION(BlueprintCallable)
	virtual void SetAllSlotsToSize(int32 SetSlotSize) override;
};
