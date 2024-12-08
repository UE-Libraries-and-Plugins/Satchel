// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Queue.h"
#include "Containers/Array.h"
#include "Satchel_Inventory.h"
#include "Components/ActorComponent.h"
#include "Core/Items/Satchel_ItemBase.h"
#include "Satchel_InventoryComponent.generated.h"

class IC_Slottable;
class AC_ReplicatedPlayerState;
class USatchel_ItemBase;

DEFINE_LOG_CATEGORY_STATIC(LogInventory, Log, All);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SATCHEL_API USatchel_InventoryComponent : public UActorComponent, public ISatchel_Inventory {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TArray<TSubclassOf<USatchel_ItemBase>> StarterItems;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	TArray<USatchel_ItemSlot*> InventorySlots;

	UPROPERTY(EditDefaultsOnly, Category = "Settings|UI")
	TSubclassOf<UUserWidget> UI_ShowItemChanged;
	
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	int32 InventoryMaxSize = 64;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	FGameplayTagContainer AllowedItemsTag;
	
public:
	// Sets default values for this component's properties
	USatchel_InventoryComponent();

	virtual USatchel_ItemSlot* GetInventorySlotAtIndex_Implementation(int32 Index) override;
	virtual USatchel_ItemSlot* GetBestSlotForItem_Implementation(const USatchel_ItemBase* Item) override;
	virtual int FindSlotIndexForExistingItem_Implementation(const FGameplayTag& ItemToFind) override;
	virtual TArray<USatchel_ItemSlot*> GetAllSlotsOfType_Implementation(const FGameplayTag& ItemTag) override;
	virtual int32 GetInventorySize_Implementation() const override {
		return InventoryMaxSize;
	}
	
	virtual TArray<USatchel_ItemBase*> ClearAllSlots_Implementation() override;

	virtual bool CanRemoveItem_Implementation(int Index) override;
	virtual const USatchel_ItemBase* RemoveItem_Implementation(int Index) override;

	virtual bool CanAcceptItem_Implementation(const USatchel_ItemBase* NewItem, int Index) override;
	virtual const USatchel_ItemBase* AddItem_Implementation(const USatchel_ItemBase* NewItem, int Index) override;
	virtual const USatchel_ItemBase* RemoveItemUnknownIndex_Implementation(const FGameplayTag& ObjectToRemove, int Count) override;
	

	// // iterator
	// typedef TCheckedPointerIterator<      TArray<USatchel_ItemSlot*>::ElementType, TArray<USatchel_ItemSlot*>::SizeType, false> RangedForIteratorType;
	// typedef TCheckedPointerIterator<const TArray<USatchel_ItemSlot*>::ElementType, TArray<USatchel_ItemSlot*>::SizeType, false> RangedForConstIteratorType;
	//
	// FORCEINLINE RangedForIteratorType begin () { return InventorySlots.begin(); }
	// FORCEINLINE RangedForConstIteratorType begin () const { return InventorySlots.begin(); }
	// FORCEINLINE RangedForIteratorType end () { return InventorySlots.end(); }
	// FORCEINLINE RangedForConstIteratorType end () const { return InventorySlots.end(); }
	
	
	/**
 * Adds a constraint to every slot in the inventory, preventing items of other types from being slotted
 * @param Constraint The constraining type
 */
	UFUNCTION(BlueprintCallable)
	void ConstrainAllSlots(TSubclassOf<USatchel_ItemBase> Constraint);

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

};
