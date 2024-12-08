// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Satchel_Statics.h"

#include "Core/Satchel_ItemInstance.h"
#include "Core/Items/Satchel_ItemBase.h"
#include "Core/Items/Satchel_LootDefinition.h"
#include "Core/Settings/Satchel_Settings.h"
#include "Kismet/GameplayStatics.h"

ASatchel_ItemInstance* USatchel_Statics::SpawnWorldItem(UObject* Context, TSubclassOf<USatchel_ItemBase> ItemToSpawn, FVector SpawnLocation, FRotator SpawnRotation) {
	const USatchel_Settings* Settings = GetDefault<USatchel_Settings>();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (Settings->bRandomDirectionSpawnVelocity) {
		SpawnRotation.Yaw = FMath::Rand() * 360.f;
		SpawnRotation.Pitch = FMath::Rand() * 360.f;
	}

	const FTransform ItemTransform(SpawnRotation, SpawnLocation);

	// If not a pawn, instigator will return nullptr which is fine. 
	ASatchel_ItemInstance* ItemInstance = Context->GetWorld()->SpawnActorDeferred<ASatchel_ItemInstance>(
		Settings->GetLoadedItemInstanceTemplate(),
		ItemTransform,
		nullptr,
		Cast<APawn>(Context),
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	
	if (IsValid(ItemInstance)) {
		ItemInstance->ItemTemplate = ItemToSpawn;
		UGameplayStatics::FinishSpawningActor(ItemInstance, ItemTransform);

		const float DirectionalMagnitude = FMath::RandRange(Settings->MinMaxDirectionalVelocity.X, Settings->MinMaxDirectionalVelocity.Y);
		const float XDir = Settings->bRandomDirectionSpawnVelocity ? FMath::RandRange(-1, 1) * DirectionalMagnitude : 0;
		const float YDir = Settings->bRandomDirectionSpawnVelocity ? FMath::RandRange(-1, 1) * DirectionalMagnitude : 0;

		const FVector ImpulseVelocity = FVector(XDir, YDir, Settings->UpwardVelocityOnSpawn);
		
		ItemInstance->AddImpulse(ImpulseVelocity, true);
	}

	return ItemInstance;
}

ASatchel_ItemInstance* USatchel_Statics::SpawnWorldItemFromLootTable(UObject* Context, const UDataTable* LootTableDataAsset, FVector Location, FRotator SpawnRotation) {
	TArray<FSatchel_LootDefinition*> LootDefinitions;
	LootTableDataAsset->GetAllRows("Completion Result Execution", LootDefinitions);

	// Add up all the weights
	int WeightSum = 0;
	for (const auto LootDefinition : LootDefinitions) {
		WeightSum += LootDefinition->Weight;
	}

	// Now we have to look again to find the item closest to our sum
	// if weight is more than or eq to rand, that's our item
	// does not support duplicate weight - You'll never see one of the items drop
	const int DesiredValue = FMath::RandRange(0, WeightSum);
	int AggregatedWeight = 0;
	for (const auto LootDefinition : LootDefinitions) {
		AggregatedWeight += LootDefinition->Weight;
		if (AggregatedWeight >= DesiredValue) {
			return SpawnWorldItem(Context, LootDefinition->ItemClass, Location, SpawnRotation);
		}
	}

	return nullptr;
}
