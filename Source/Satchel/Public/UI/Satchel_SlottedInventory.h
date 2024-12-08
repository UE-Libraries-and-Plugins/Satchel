// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Satchel_SlottedInventory.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USatchel_SlottedInventory : public UInterface {
	GENERATED_BODY()
};

/**
 * 
 */
class SATCHEL_API ISatchel_SlottedInventory {
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	virtual void SetAllSlotsToSize(int32 SetSlotSize = 0) = 0;
	
};
