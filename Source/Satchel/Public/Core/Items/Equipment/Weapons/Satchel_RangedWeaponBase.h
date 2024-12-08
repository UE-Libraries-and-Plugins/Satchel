// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Satchel_WeaponBase.h"
#include "Satchel_RangedWeaponBase.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class SATCHEL_API USatchel_RangedWeaponBase : public USatchel_WeaponBase {
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess))
	TSubclassOf<AActor> BaseProjectile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess))
	FString ProjectileSpawnSocketName = TEXT("ProjectileSpawn");

public:

	UFUNCTION(BlueprintCallable)
	const AActor* GetBaseProjectile() const;

	UFUNCTION(BlueprintCallable)
	virtual UStaticMeshSocket* GetProjectileSpawnSocket();
};
