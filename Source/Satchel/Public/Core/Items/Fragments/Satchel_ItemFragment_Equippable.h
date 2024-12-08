// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Satchel_ItemFragment_Base.h"
#include "Satchel_ItemFragment_Equippable.generated.h"

class USatchel_EquipmentComponent;
class ISatchel_Inventory;
/**
 * 
 */
UCLASS()
class SATCHEL_API USatchel_ItemFragment_Equippable : public USatchel_ItemFragment_Base {
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category=Debug)
	bool bIsEquipped;
	
	UPROPERTY(VisibleAnywhere, Category=Debug)
	USatchel_EquipmentComponent* OwningInventory;

	UPROPERTY(VisibleAnywhere, Category=Debug)
	AActor* OwningActor;
	
public:
	
	/// Informs the item that is has been equipped
	/// @param EquipmentInventory 
	UFUNCTION(BlueprintCallable, Category="Item|Fragment|Equip")
	void Equip(USatchel_EquipmentComponent* EquipmentInventory);

	/// Informs the item that is has been unequipped
	/// The purpose is to remove all references to the equipped item and owning actor
	UFUNCTION(BlueprintCallable, Category="Item|Fragment|Equip")
	void UnEquip();

	UFUNCTION(BlueprintCallable, Category="Item|Fragment|Equip")
	bool IsEquipped() const {
		return bIsEquipped;
	}

	UFUNCTION(BlueprintCallable, Category="Item|Fragment|Equip")
	USatchel_EquipmentComponent* GetOwningInventory() {
		return OwningInventory;
	}

	UFUNCTION(BlueprintCallable, Category="Item|Fragment|Equip")
	AActor* GetOwningActor() {
		return OwningActor;
	}
};
