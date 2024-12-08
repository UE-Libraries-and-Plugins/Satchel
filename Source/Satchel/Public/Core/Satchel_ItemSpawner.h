// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Items/Satchel_LootDefinition.h"
#include "Satchel_ItemSpawner.generated.h"

UCLASS()
class SATCHEL_API ASatchel_ItemSpawner : public AActor {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Loot")
	bool ShouldSpawnFromLootTable;
	
	UPROPERTY(EditAnywhere, Category = "Loot", meta=(EditCondition="ShouldSpawnFromLootTable", EditConditionHides))
	UDataTable* LootDefinition;

	UPROPERTY(EditAnywhere, Category = "Loot", meta=(EditCondition="ShouldSpawnFromLootTable==False", EditConditionHides))
	TSubclassOf<USatchel_ItemBase> ItemToSpawn;

	UPROPERTY(EditAnywhere)
	UBoxComponent* SpawnerZone;

	/// Spawns every X seconds. A value of 0.f will cause the items to spawn immediately.
	/// Do not set this to 0 if ShouldLoopSpawn is active
	UPROPERTY(EditAnywhere, Category = "Loot|Settings")
	float SpawnAtIntervals = 0.1f;

	/// If true, will continue to spawn items every interval
	UPROPERTY(EditAnywhere, Category = "Loot|Settings")
	bool ShouldLoopSpawn;
	
	UPROPERTY(EditAnywhere, Category = "Loot|Settings")
	int NumberOfItemsToSpawn = 1;

	/// Maximum number of actors that can be in the spawning zone before spawning ends (temporarily)
	UPROPERTY(EditAnywhere, Category = "Loot|Settings")
	int MaximumItemsInZone = 10;
	
	UPROPERTY(EditAnywhere, Category = "Loot|Settings")
	bool DoNotSpawnWhenLocalPlayerIsInSpawningBox = true;

	UPROPERTY(EditAnywhere, Category = "Loot|Settings", meta=(EditCondition="DoNotSpawnWhenLocalPlayerIsInSpawningBox", EditConditionHides))
	UClass* PlayerCharacterClass;
	
public:
	// Sets default values for this actor's properties
	ASatchel_ItemSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnItem();

};
