// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/Components/Satchel_InventoryComponent.h"
#include "Interface/Context_Giver.h"
#include "Satchel_InventorySlotWidget.generated.h"

class USatchel_InventorySlotItemWidget;
class UCanvasPanel;
class UCanvasPanelSlot;
class UBorder;
class USatchel_ItemBase;

DECLARE_MULTICAST_DELEGATE()

/**
 * 
 */
UCLASS()
class SATCHEL_API USatchel_InventorySlotWidget : public UUserWidget, public IContext_Giver {
	GENERATED_BODY()

private:
	
	UPROPERTY()
	USatchel_ItemSlot* CachedInventorySlot;
	
	UPROPERTY()
	USatchel_InventorySlotItemWidget* SlotItemWidget;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> SlotBorder;

	UPROPERTY()
	FVector2D SlotSize;
	
	UPROPERTY(EditDefaultsOnly)
	TSet<UContext_ActionEntry*> ContextEntries;

	UPROPERTY(EditDefaultsOnly)
	UContext_ActionEntry* PrimaryContext;
	
public:

	virtual void GetGiverContextEntries_Implementation(TSet<UContext_ActionEntry*>& OutContextEntries) override;
	virtual UContext_ActionEntry* GetPrimaryContextEntry_Implementation() override;
	
	/**
	 * 
	 */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> SlotItemWidgetTemplate;

	/**
	 * 
	 * @param Height 
	 * @param Width 
	 */
	UFUNCTION(BlueprintCallable)
	void SetSize(int32 Height, int32 Width);

	UFUNCTION(BlueprintCallable)
	FVector2D GetSize() const;
	
	/**
	 * 
	 * @param InventorySlot 
	 */
	UFUNCTION(BlueprintCallable)
	void SetInventorySlot(USatchel_ItemSlot* InventorySlot);

	/**
	 * 
	 * @return 
	 */
	UFUNCTION(BlueprintCallable)
	const USatchel_ItemSlot* GetInventorySlot() const;

	UFUNCTION(BlueprintCallable)
	void PrepareSlot();

	UFUNCTION(BlueprintCallable)
	void Refresh();

	void Refresh(const USatchel_ItemBase* Old, const USatchel_ItemBase* New);

private:
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
