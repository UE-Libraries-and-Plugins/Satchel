// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Satchel_ItemSpawner.h"

#include "Core/Satchel_ItemInstance.h"
#include "Core/Satchel_Statics.h"
#include "Core/Items/Satchel_ItemBase.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ASatchel_ItemSpawner::ASatchel_ItemSpawner() {
	SpawnerZone = CreateDefaultSubobject<UBoxComponent>("Spawner Zone");
}

// Called when the game starts or when spawned
void ASatchel_ItemSpawner::BeginPlay() {
	Super::BeginPlay();

	FTimerHandle SpawnerHandle;
	GetWorldTimerManager().SetTimer(SpawnerHandle, this, &ASatchel_ItemSpawner::SpawnItem, SpawnAtIntervals, ShouldLoopSpawn);
}

void ASatchel_ItemSpawner::SpawnItem() {

	if (DoNotSpawnWhenLocalPlayerIsInSpawningBox) {
		TArray<AActor*> PlayerActors;
		SpawnerZone->GetOverlappingActors(PlayerActors, PlayerCharacterClass);
		//  Iterate all of class and validate if it is a local player
		for (const auto PlayerActor : PlayerActors) {
			if (const ACharacter* PlayerCharacter = Cast<ACharacter>(PlayerActor);
				IsValid(PlayerCharacter)) {
				if (PlayerCharacter->GetController()->IsLocalPlayerController()) {
					return;
				}
			}
		}
	}
	
	TArray<AActor*> ExistingItems;
	SpawnerZone->GetOverlappingActors(ExistingItems, ASatchel_ItemInstance::StaticClass());
	if (ExistingItems.Num() >= MaximumItemsInZone) return;

	const FVector SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(SpawnerZone->Bounds.Origin, SpawnerZone->Bounds.BoxExtent);
	
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	if (ShouldSpawnFromLootTable) {
		USatchel_Statics::SpawnWorldItemFromLootTable(this, LootDefinition, SpawnLocation, GetActorRotation());
	} else {
		USatchel_Statics::SpawnWorldItem(this, ItemToSpawn, SpawnLocation, GetActorRotation());
	}
}



