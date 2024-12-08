// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Interface/Context_Giver.h"
#include "UI/Satchel_SlottedInventory.h"
#include "Satchel_EquipmentWidget.generated.h"

#define SLOT_DEFINITION(SlotName) \
	UPROPERTY(meta = (BindWidget)) \
	TObjectPtr<USatchel_InventorySlotWidget> SlotName##Slot; \
	UPROPERTY(EditDefaultsOnly, Category = "Settings|Equipment") \
	FGameplayTag SlotName##SlotTag;

#define SLOT_SETUP(SlotName) \
	SlotName##Slot->SetInventorySlot(LinkedInventory->GetInventorySlotOfType(SlotName##SlotTag)); \
	SlotName##Slot->PrepareSlot();

class USatchel_EquipmentComponent;
class USatchel_InventorySlotWidget;
/**
 * 
 */
UCLASS()
class SATCHEL_API USatchel_EquipmentWidget : public UUserWidget, public ISatchel_SlottedInventory, public IContext_Giver {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSet<UContext_ActionEntry*> ContextEntries;
	
public:

	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	uint32 SlotSize = 64;
	
	UFUNCTION(BlueprintCallable)
	virtual void SetAllSlotsToSize(int32 SetSlotSize) override;

	UFUNCTION(BlueprintCallable)
	void RegenerateSlots(USatchel_EquipmentComponent* LinkedInventory);

	UFUNCTION(BlueprintCallable)
	virtual void NativeOnInitialized() override;

	virtual void GetGiverContextEntries_Implementation(TSet<UContext_ActionEntry*>& OutContextEntries) override;

private:

	UPROPERTY(meta = (BindWidget)) 
	TObjectPtr<USatchel_InventorySlotWidget> HeadSlot; 
	UPROPERTY(EditDefaultsOnly, Category = "Settings|Equipment") 
	FGameplayTag HeadSlotTag;

	UPROPERTY(meta = (BindWidget)) 
	TObjectPtr<USatchel_InventorySlotWidget> ChestSlot; 
	UPROPERTY(EditDefaultsOnly, Category = "Settings|Equipment") 
	FGameplayTag ChestSlotTag;
	
	UPROPERTY(meta = (BindWidget)) 
	TObjectPtr<USatchel_InventorySlotWidget> LegSlot; 
	UPROPERTY(EditDefaultsOnly, Category = "Settings|Equipment")
	FGameplayTag LegSlotTag;
	
	UPROPERTY(meta = (BindWidget)) 
	TObjectPtr<USatchel_InventorySlotWidget> WeaponSlot; 
	UPROPERTY(EditDefaultsOnly, Category = "Settings|Equipment") 
	FGameplayTag WeaponSlotTag;

	UPROPERTY(meta = (BindWidget)) 
	TObjectPtr<USatchel_InventorySlotWidget> BootSlot; 
	UPROPERTY(EditDefaultsOnly, Category = "Settings|Equipment") 
	FGameplayTag BootSlotTag;
	
	UPROPERTY(meta = (BindWidget)) 
	TObjectPtr<USatchel_InventorySlotWidget> HandSlot; 
	UPROPERTY(EditDefaultsOnly, Category = "Settings|Equipment") 
	FGameplayTag HandSlotTag;

	UPROPERTY(meta = (BindWidget)) 
	TObjectPtr<USatchel_InventorySlotWidget> ToolASlot; 
	UPROPERTY(EditDefaultsOnly, Category = "Settings|Equipment") 
	FGameplayTag ToolASlotTag;
};
