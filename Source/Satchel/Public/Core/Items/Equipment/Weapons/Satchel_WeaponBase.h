// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Items/Equipment/Satchel_EquipmentBase.h"
#include "Satchel_WeaponBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SATCHEL_API USatchel_WeaponBase : public USatchel_EquipmentBase {
	GENERATED_BODY()

public:
	USatchel_WeaponBase();
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess))
	float BasicAbilityCooldown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,  Category = "Mesh", meta = (AllowPrivateAccess))
	FString HandConnectSocketName = TEXT("HandConnect");
	
public:

	UFUNCTION(BlueprintCallable)
	virtual UStaticMeshSocket* GetHandConnectionSocket() const;
};
