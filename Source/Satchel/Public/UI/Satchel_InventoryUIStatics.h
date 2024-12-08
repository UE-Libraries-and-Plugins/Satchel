// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Satchel_InventoryUIStatics.generated.h"

USTRUCT(BlueprintType)
struct SATCHEL_API FItemPopupPayload {
	GENERATED_BODY()

	UPROPERTY()
	UTexture2D* Icon;

	UPROPERTY()
	FText PopupName;
	
	UPROPERTY()
	bool bWasRemoved = false;
};

/**
 * 
 */
UCLASS()
class SATCHEL_API USatchel_InventoryUIStatics : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	UFUNCTION()
	static void SpawnItemInventoryPopup(const FItemPopupPayload& Item, const AActor* FollowActor);
};
