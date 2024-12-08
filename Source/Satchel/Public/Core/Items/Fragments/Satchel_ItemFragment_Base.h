// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Satchel_ItemFragment_Base.generated.h"

class USatchel_ItemSlot;
class USatchel_ItemBase;
/**
 * 
 */
UCLASS(EditInlineNew, Abstract, Blueprintable)
class SATCHEL_API USatchel_ItemFragment_Base : public UObject {
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void OnItemInitialized(USatchel_ItemBase* Item) {}

	UFUNCTION()
	virtual bool OnCanAccept(const USatchel_ItemBase* Item, const USatchel_ItemSlot* Slot) const { return true; }
};
