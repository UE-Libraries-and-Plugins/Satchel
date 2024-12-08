// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Satchel_Settings.generated.h"

class ASatchel_ItemInstance;
/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Satchel Settings"))
class SATCHEL_API USatchel_Settings : public UDeveloperSettings {
	GENERATED_BODY()

public:

	UPROPERTY(Config, EditAnywhere, Category = "Settings|Defaults")
	float UpwardVelocityOnSpawn = 700.0;

	/**
	 * If random direction spawn is true, this will set the velocity.
	 * Basically, how far min/max an object can go in its random direction
	 */
	UPROPERTY(Config, EditAnywhere, Category = "Settings|Defaults")
	FVector2D MinMaxDirectionalVelocity = FVector2D(100, 200);

	/**
	 * If an object should also have a Y velocity. If false, object will only move upwards on spawn
	 */
	UPROPERTY(Config, EditAnywhere, Category = "Settings|Defaults")
	bool bRandomDirectionSpawnVelocity = true;

	UFUNCTION(BlueprintCallable)
	TSubclassOf<ASatchel_ItemInstance> GetLoadedItemInstanceTemplate(bool ForceReload = false) const;

	UFUNCTION(BlueprintCallable)
	TSubclassOf<UUserWidget> GetLoadedItemChangePopupTemplate(bool ForceReload = false) const;
	
protected:
	UPROPERTY(Config, EditAnywhere, Category = "Settings|Templates")
	TSoftClassPtr<ASatchel_ItemInstance> ItemInstanceTemplate;
	
	UPROPERTY(Config, EditAnywhere, Category = "Settings|Templates|UI")
	TSoftClassPtr<UUserWidget> ItemChangePopupTemplate;
};
