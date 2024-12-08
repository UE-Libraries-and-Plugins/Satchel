// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Satchel_InventoryComponent.h"
#include "Components/ActorComponent.h"
#include "Core/Items/Equipment/Satchel_EquipmentBase.h"
#include "Core/Items/Equipment/Satchel_EquipmentSlotEnum.h"
#include "Satchel_EquipmentComponent.generated.h"

class UAbilitySystemComponent;
struct FGameplayAbilitySpecHandle;
struct FActiveGameplayEffectHandle;
class USatchel_EquipmentBase;

DEFINE_LOG_CATEGORY_STATIC(LogCharacterEquipment, Log, All);


UCLASS(Blueprintable)
class SATCHEL_API USatchel_EquipmentSlot : public USatchel_ItemSlot {
	GENERATED_BODY()

public:

	// The mesh component whos mesh will be replaced - Must be cast to proper mesh type
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UMeshComponent* EquipmentMeshComponent;

	// Meshes that can be deactivated, or will cause clipping
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UMeshComponent* MeshToDeactivateOnEquip;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> ActiveEffectHandles;

	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> ActiveAbilityHandles;
	
};

UCLASS(BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SATCHEL_API USatchel_EquipmentComponent : public UActorComponent, public ISatchel_Inventory {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<USatchel_EquipmentSlot*> EquipmentSlots;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Equipment", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* RightHandHeldMeshComponent;

	UPROPERTY(VisibleAnywhere, Category="GAS")
	UAbilitySystemComponent* AbilitySystemComponent;
	
public:
	// Sets default values for this component's properties
	USatchel_EquipmentComponent();

	/**
	 * Get a slot containing the specified piece of gear
	 */
	UFUNCTION(BlueprintCallable)
	USatchel_EquipmentSlot* GetInventorySlotOfType(FGameplayTag Type);

	///////////////
	// Interface
	virtual USatchel_ItemSlot* GetBestSlotForItem_Implementation(const USatchel_ItemBase* Item) override;
	virtual int32 GetInventorySize_Implementation() const override;
	virtual USatchel_ItemSlot* GetInventorySlotAtIndex_Implementation(int32 Index) override;
	virtual int FindSlotIndexForExistingItem_Implementation(const FGameplayTag& ItemToFind) override;
	virtual TArray<USatchel_ItemBase*> ClearAllSlots_Implementation() override;

	virtual bool CanRemoveItem_Implementation(int Index) override;
	virtual const USatchel_ItemBase* RemoveItem_Implementation(int Index) override;

	virtual bool CanAcceptItem_Implementation(const USatchel_ItemBase* NewItem, int Index) override;
	virtual const USatchel_ItemBase* AddItem_Implementation(const USatchel_ItemBase* NewItem, int Index) override;
	virtual const USatchel_ItemBase* RemoveItemUnknownIndex_Implementation(const FGameplayTag& ObjectToRemove, int Count) override;
	virtual TArray<USatchel_ItemSlot*> GetAllSlotsOfType_Implementation(const FGameplayTag& ItemTag) override;
	//// Interface end
	//
	// // Iterator
	// typedef TCheckedPointerIterator<      TArray<USatchel_EquipmentSlot*>::ElementType, TArray<USatchel_EquipmentSlot*>::SizeType, false> RangedForIteratorType;
	// typedef TCheckedPointerIterator<const TArray<USatchel_EquipmentSlot*>::ElementType, TArray<USatchel_EquipmentSlot*>::SizeType, false> RangedForConstIteratorType;
	//
	// FORCEINLINE RangedForIteratorType begin () { return EquipmentSlots.begin(); }
	// FORCEINLINE RangedForConstIteratorType begin () const { return EquipmentSlots.begin(); }
	// FORCEINLINE RangedForIteratorType end () { return EquipmentSlots.end(); }
	// FORCEINLINE RangedForConstIteratorType end () const { return EquipmentSlots.end(); }
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable)
	void SetupComponent(UAbilitySystemComponent* ASC);

	/**
	 * Try to get the currently equipped gear for a certain slot.
	 * If nothing is equipped, returns nullptr
	 */
	UFUNCTION(BlueprintCallable)
	const USatchel_EquipmentBase* GetEquipmentForSlot(const FGameplayTag SlotEnum);
	
	UFUNCTION(BlueprintCallable)
	void AttachMeshToPlayerHands(UStaticMesh* MeshToAttach, const FVector& AttachPosition) const;

	UFUNCTION(BlueprintCallable)
	void DetachMeshFromPlayerHands() const;

	UFUNCTION(BlueprintCallable)
	UStaticMeshComponent* GetHeldItemStaticMeshComponent() const;

	UFUNCTION(BlueprintCallable)
	void SetHeldItemStaticMeshComponent(UStaticMeshComponent* StaticMeshComponent);

	UFUNCTION(BlueprintCallable)
	bool AddEquipmentSlot(USatchel_EquipmentSlot* EquipmentSlot);

	UFUNCTION(BlueprintCallable)
	void SetupEquipmentSlot(USatchel_EquipmentSlot* EquipmentSlot, int Index);
	
	/**
	 * Helper function for constructing a new equipment slot in Blueprint
	 * @param SlotEnum 
	 * @param EquipmentMeshComponent 
	 * @param MeshToDeactivateOnEquip 
	 * @param BaseEquipment 
	 * @param IsLocked 
	 * @return 
	 */
	UFUNCTION(BlueprintCallable)
	USatchel_EquipmentSlot* MakeEquipmentSlot(
		FGameplayTagContainer AllowedItems,
		USkeletalMeshComponent* EquipmentMeshComponent,
		USkeletalMeshComponent* MeshToDeactivateOnEquip = nullptr,
		USatchel_EquipmentBase* BaseEquipment = nullptr,
		bool IsLocked = false);

	/**
	 * Helper function for constructing a new equipment slot in  blueprint
	 * @param SlotEnum 
	 * @param EquipmentMeshComponent 
	 * @param MeshToDeactivateOnEquip 
	 * @param BaseEquipment 
	 * @param IsLocked 
	 * @return 
	 */
	UFUNCTION(BlueprintCallable)
	USatchel_EquipmentSlot* MakeEquipmentSlotGeneric(
		FGameplayTagContainer AllowedItems,
		UMeshComponent* EquipmentMeshComponent,
		UMeshComponent* MeshToDeactivateOnEquip = nullptr,
		USatchel_EquipmentBase* BaseEquipment = nullptr,
		bool IsLocked = false);

protected:
	/**
	 * Equip an item to the specified slot
	 * @return Existing equipment
	 */
	virtual const USatchel_EquipmentBase* Equip(USatchel_EquipmentSlot* Slot, const USatchel_EquipmentBase* EquipmentItem);
	
	/**
	 * Unequip the item equipped on a specific slot
	 * @return Removed item
	 */
	virtual const USatchel_EquipmentBase* UnEquip(USatchel_EquipmentSlot* Slot);
	
private:
	UFUNCTION()
	bool ApplyGameplayEffects(const USatchel_ItemBase* Item, USatchel_EquipmentSlot* EquipmentSlot) const;

	UFUNCTION()
	bool RemoveGameplayEffects(USatchel_EquipmentSlot* EquipmentSlot) const;

	UFUNCTION()
	bool GrantGameplayAbilities(const USatchel_ItemBase* Item, USatchel_EquipmentSlot* EquipmentSlot) const;

	UFUNCTION()
	bool RevokeGameplayAbilities(USatchel_EquipmentSlot* EquipmentSlot) const;
};
