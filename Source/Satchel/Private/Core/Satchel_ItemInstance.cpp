// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Satchel_ItemInstance.h"

#include "Satchel.h"
#include "Satchel_InventoryStatics.h"
#include "Actions/Context_Action.h"
#include "Actions/Context_ActionEntry.h"
#include "Actions/Context_ActionSubsystem.h"
#include "Components/BoxComponent.h"
#include "Components/Context_HolderComponent.h"
#include "Core/Components/Satchel_InventoryComponent.h"
#include "Core/Components/Satchel_InventoryHolder.h"
#include "Core/Items/Satchel_ItemBase.h"
#include "GameFramework/Character.h"
#include "UI/Satchel_InventoryUIStatics.h"

// Sets default values
ASatchel_ItemInstance::ASatchel_ItemInstance() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>("Box Collider");
	RootComponent = BoxCollider;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Item Mesh");
	StaticMeshComponent->SetupAttachment(RootComponent);

	InteractionCollider = CreateDefaultSubobject<UBoxComponent>("Interaction Collider");
	InteractionCollider->SetupAttachment(StaticMeshComponent);
	InteractionCollider->OnComponentBeginOverlap.AddDynamic(this, &ASatchel_ItemInstance::InteractionBeginOverlap);
	InteractionCollider->OnComponentEndOverlap.AddDynamic(this, &ASatchel_ItemInstance::InteractionEndOverlap);

	ContextComponent = CreateDefaultSubobject<UContext_HolderComponent>("Context Component");
}

// Called when the game starts or when spawned
void ASatchel_ItemInstance::BeginPlay() {
	Super::BeginPlay();

	if (IsValid(ItemTemplate)) {
		SetItemData(ItemTemplate);
	}
}

bool ASatchel_ItemInstance::TryPickUp(AActor* InstigatingActor) {
/////////
// ~VALIDATION

	// Item valid
	if (!IsValid(ItemData)) {
		UE_LOG(LogSatchel, Error, TEXT("Item data for %s is invalid. Item cannot be picked up"),
			*GetName());
		return false;
	}

	// Actor has interface
	if (!InstigatingActor->Implements<USatchel_InventoryHolder>()) {
		UE_LOG(LogSatchel, Warning, TEXT("Attempting to pick up item %s on actor %s, but actor does not implement %s"),
			*GetName(),
			*InstigatingActor->GetName(),
			*USatchel_InventoryHolder::StaticClass()->GetName())
		return false;
	}
	
	// Inventory valid
	UObject* Inventory =  ISatchel_InventoryHolder::Execute_GetInventory(InstigatingActor);
	if (!ensure(Inventory)) {
		UE_LOG(LogSatchel, Warning, TEXT("Attempting to pick up item %s on actor %s, but actor inventory is invalid"),
			*GetName(),
			*InstigatingActor->GetName())
		return false;
	}

	// Can take item
	if(!ISatchel_Inventory::Execute_CanAcceptItem(Inventory, ItemData, -1)) {
		UE_LOG(LogSatchel, Log, TEXT("%s cannot pick up %s"),
			*InstigatingActor->GetName(),
			*ItemData->DisplayName.ToString())
		return false;
	}

/////////
// !VALIDATION
	
	USatchel_InventoryStatics::GiveInventoryItem(Inventory, 1, ItemData, -1);
	
	if (DestroyOnPickup) {
		StartDestroy();
	}

	return true;
}

void ASatchel_ItemInstance::SetItemData(TSubclassOf<USatchel_ItemBase> InstanceTemplate) {

	ItemData = NewObject<USatchel_ItemBase>(this, InstanceTemplate);
	ContextComponent->SetDisplayName(ItemData->DisplayName);

	const UStaticMesh* Mesh = ItemData->GetStaticMesh();
	if (ensure(Mesh)) {
		StaticMeshComponent->SetStaticMesh(ItemData->GetStaticMesh());
	} 
}

void ASatchel_ItemInstance::AddImpulse(const FVector Impulse, const bool VelChange) const {
	StaticMeshComponent->AddImpulse(Impulse, NAME_None, VelChange);
}

void ASatchel_ItemInstance::InteractionBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
                                                    UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult) {
	ACharacter* Character = Cast<ACharacter>(Actor);
	if (!IsValid(Character)) return;
	if (Character->IsLocallyControlled() && Character->IsPlayerControlled()) {
		Execute_EnterInteractionZone(this, Character);
	}
}

void ASatchel_ItemInstance::InteractionEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
                                                  UPrimitiveComponent* PrimitiveComponent1, int I) {
	ACharacter* Character = Cast<ACharacter>(Actor);
	if (!IsValid(Character)) return;
	if (Character->IsLocallyControlled() && Character->IsPlayerControlled()) {
		Execute_ExitInteractionZone(this, Character);
	}
}

void ASatchel_ItemInstance::StartDestroy() {
	Destroy();
}

bool ASatchel_ItemInstance::Interact_Implementation(AActor* ActorInteracting) {
	if (!Execute_CanInteract(this, ActorInteracting)) return false;

	UContext_ActionSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UContext_ActionSubsystem>();
	const UContext_ActionEntry* ActionEntry = Subsystem->GetPrimaryContextEntryForObject(ContextComponent);
	if (IsValid(ActionEntry)) {
		return Subsystem->ExecuteAction(ContextComponent, ActionEntry, ActorInteracting);
	}
			
	return false;
}

bool ASatchel_ItemInstance::CanInteract_Implementation(AActor* ActorInteracting) {
	if (bLockItem) return false;
	if (!ActorInteracting->Implements<USatchel_InventoryHolder>()) return false;

	USatchel_InventoryComponent* Comp = ISatchel_InventoryHolder::Execute_GetInventory(ActorInteracting);
	return bCanInteract && ISatchel_Inventory::Execute_CanAcceptItem(Comp, ItemData, -1);
}


void ASatchel_ItemInstance::EnterInteractionZone_Implementation(AActor* ActorEnteringZone) {
	if (!HasAuthority()) return;
	bCanInteract = true;
}

void ASatchel_ItemInstance::ExitInteractionZone_Implementation(AActor* ActorExitingZone) {
	if (!HasAuthority()) return;
	bCanInteract = false;
}

