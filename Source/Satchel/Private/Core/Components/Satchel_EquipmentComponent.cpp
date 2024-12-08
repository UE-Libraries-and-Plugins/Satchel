// Fill out your copyright notice in the Description page of Project Settings.


#include "Core\Components\Satchel_EquipmentComponent.h"

#include <assert.h>

#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "Core/Items/Equipment/Satchel_EquipmentBase.h"
#include "Core/Items/Equipment/Weapons/Satchel_WeaponBase.h"
#include "Core/Items/Fragments/Satchel_ItemFragment_Equippable.h"
#include "Core/Items/Fragments/Satchel_ItemFragment_Requirements.h"
#include "Core/Items/Fragments/Satchel_ItemFragment_SkeletalMesh.h"
#include "Core/Items/Fragments/Satchel_ItemFragment_StaticMesh.h"
#include "Engine/StaticMeshSocket.h"

// Sets default values for this component's properties
USatchel_EquipmentComponent::USatchel_EquipmentComponent() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

int32 USatchel_EquipmentComponent::GetInventorySize_Implementation() const {
	return EquipmentSlots.Num();
}

USatchel_EquipmentSlot* USatchel_EquipmentComponent::GetInventorySlotOfType(FGameplayTag Type) {

	// Finds the best (closest) match to the provided type
	// we do this by looping through all the tags set to the slot, which act as filters
	// Not very performant. Too many tags could massively impact performance 
	USatchel_EquipmentSlot* BestSlot = nullptr;
	int32 MatchScore = 0;
	for (const auto Slot : EquipmentSlots) {
		for (const  auto AllowedItem : Slot->AllowedItems) {
			if (Type.MatchesTag(AllowedItem)) {
				const int32 CurrentMatchScore = Type.MatchesTagDepth(AllowedItem);
				if (CurrentMatchScore > MatchScore) {
					MatchScore = CurrentMatchScore;
					BestSlot = Slot;
				}
			}	
		}
	}
	return BestSlot;
}

USatchel_ItemSlot* USatchel_EquipmentComponent::GetBestSlotForItem_Implementation(const USatchel_ItemBase* Item) {
	const USatchel_EquipmentBase* Equipment = Cast<USatchel_EquipmentBase>(Item);

	if (!IsValid(Equipment) || !IsValid(Equipment->FindFragment<USatchel_ItemFragment_Equippable>())) {
		return nullptr;
	}
	
	return GetInventorySlotOfType(Equipment->ItemId);
}

bool USatchel_EquipmentComponent::CanRemoveItem_Implementation(int Index) {
	return !Execute_GetInventorySlotAtIndex(this, Index)->bIsLocked;
}

const USatchel_ItemBase* USatchel_EquipmentComponent::RemoveItem_Implementation(int Index) {
	USatchel_EquipmentSlot* EquipmentSlot = Cast<USatchel_EquipmentSlot>(Execute_GetInventorySlotAtIndex(this, Index));
	if (!IsValid(EquipmentSlot)) {
		return nullptr;
	}

	// remove gear if slot is valid
	return UnEquip(EquipmentSlot);
}

bool USatchel_EquipmentComponent::CanAcceptItem_Implementation(const USatchel_ItemBase* NewItem, int Index) {

	if (!IsValid(NewItem)) return true;
	
	const USatchel_EquipmentBase* NewEquipment = Cast<USatchel_EquipmentBase>(NewItem);
	if (!IsValid(NewEquipment) || !IsValid(NewEquipment->FindFragment<USatchel_ItemFragment_Equippable>())) {
		return false;
	}
	
	// Check stat requirements 
	const USatchel_ItemFragment_Requirements* RequirementFragment = NewEquipment->FindFragment<USatchel_ItemFragment_Requirements>();
	if (IsValid(RequirementFragment)) {
		for (const auto Requirement : RequirementFragment->UsageRequirements) {
			bool bFound;
			const float AttributeValue = AbilitySystemComponent->GetGameplayAttributeValue(Requirement.Key, bFound);
			if (!bFound || AttributeValue < Requirement.Value) {
				return false;
			} 
		}
	}
	
	const USatchel_EquipmentSlot* EquipmentSlot = Index == -1 ?
		                                              GetInventorySlotOfType(NewEquipment->ItemId) :
		                                              Cast<USatchel_EquipmentSlot>(Execute_GetInventorySlotAtIndex(this, Index));
	
	return (
		IsValid(EquipmentSlot) &&
		!EquipmentSlot->bIsLocked &&
		EquipmentSlot->CanAcceptItem(NewItem));
}

const USatchel_ItemBase* USatchel_EquipmentComponent::AddItem_Implementation(const USatchel_ItemBase* NewItem, int Index) {

	// if item is null, we're probably just trying to remove the existing item
	if (!IsValid(NewItem)) {
		return Execute_RemoveItem(this, Index);
	}
	
	const USatchel_EquipmentBase* NewEquipment = Cast<USatchel_EquipmentBase>(NewItem);

	if (!IsValid(NewEquipment) || !IsValid(NewEquipment->FindFragment<USatchel_ItemFragment_Equippable>())) {
		return nullptr;
	}
	
	USatchel_EquipmentSlot* EquipmentSlot = Index == -1 ?
		GetInventorySlotOfType(NewEquipment->ItemId) :
		Cast<USatchel_EquipmentSlot>(Execute_GetInventorySlotAtIndex(this, Index));

	if (!IsValid(EquipmentSlot)) {
		return nullptr;
	}

	// Equip and replace gear if valid
	return Equip(EquipmentSlot, NewEquipment);
}

const USatchel_ItemBase* USatchel_EquipmentComponent::RemoveItemUnknownIndex_Implementation(
	const FGameplayTag& ObjectToRemove,
	int Count) {
	if (ObjectToRemove.IsValid()) {
		const USatchel_EquipmentSlot* Slot = GetInventorySlotOfType(ObjectToRemove);
		if (IsValid(Slot)) {
			return Execute_RemoveItem(this, Slot->GetSlotIndex());
		}
	}
	return nullptr;
}

TArray<USatchel_ItemSlot*> USatchel_EquipmentComponent::GetAllSlotsOfType_Implementation(const FGameplayTag& ItemTag) {
	TArray<USatchel_ItemSlot*> Slots;
	
	for (const auto Slot : EquipmentSlots) {
		for (const auto AllowedItem : Slot->AllowedItems) {
			if (AllowedItem.MatchesTag(ItemTag)) {
				Slots.Add(Slot);
			}	
		}
	}
	return Slots;
}

USatchel_ItemSlot* USatchel_EquipmentComponent::GetInventorySlotAtIndex_Implementation(int32 Index) {
	if (EquipmentSlots.Num() <= Index) return nullptr;

	if (Index == -1) return nullptr;
	
	return EquipmentSlots[Index];
}

int USatchel_EquipmentComponent::FindSlotIndexForExistingItem_Implementation(const FGameplayTag& ItemToFind) {
	if (!ItemToFind.IsValid()) return -1;
	
	const USatchel_EquipmentSlot* Slot = GetInventorySlotOfType(ItemToFind);
	if (!IsValid(Slot)) return -1;
	
	return Slot->GetSlotIndex();
}

TArray<USatchel_ItemBase*> USatchel_EquipmentComponent::ClearAllSlots_Implementation() {
	TArray<USatchel_ItemBase*> ExistingItems;

	for(USatchel_ItemSlot* Slot : EquipmentSlots) {
		if (IsValid(Slot->PeekItem())) {
			USatchel_EquipmentSlot* EquipmentSlot = Cast<USatchel_EquipmentSlot>(Slot);

			const USatchel_EquipmentBase* ExistingItem = UnEquip(EquipmentSlot);

			if (IsValid(ExistingItem)) {
				ExistingItems.Add(const_cast<USatchel_EquipmentBase*>(ExistingItem));
			}
		}
	}

	return ExistingItems;
}

// Called when the game starts
void USatchel_EquipmentComponent::BeginPlay() {
	Super::BeginPlay();

	for (int i = 0; i < EquipmentSlots.Num(); i++) {
		SetupEquipmentSlot(EquipmentSlots[i], i);
	}
}

void USatchel_EquipmentComponent::SetupComponent(UAbilitySystemComponent* ASC) {
	if (GetOwner()->GetLocalRole() != ROLE_Authority) return;

	AbilitySystemComponent = ASC;
}

const USatchel_EquipmentBase* USatchel_EquipmentComponent::GetEquipmentForSlot(const FGameplayTag SlotType) {
	const USatchel_EquipmentSlot* FoundSlot = GetInventorySlotOfType(SlotType);
	if(IsValid(FoundSlot)) {
		if (!IsValid(FoundSlot)) {
			UE_LOG(LogCharacterEquipment, Error, TEXT("Equipment slot is invalid!!"))
		}

		return Cast<USatchel_EquipmentBase>(FoundSlot->PeekItem());
	}
	
	UE_LOG(LogCharacterEquipment, Error, TEXT("Could not find requested equipment slot of %s"), *SlotType.ToString())
	return nullptr;
}

#pragma region Mesh

void USatchel_EquipmentComponent::AttachMeshToPlayerHands(UStaticMesh* MeshToAttach, const FVector& AttachPosition) const {
	if (IsValid(RightHandHeldMeshComponent)) {
		RightHandHeldMeshComponent->SetStaticMesh(MeshToAttach);
		RightHandHeldMeshComponent->SetRelativeLocation(AttachPosition);
	}
}

void USatchel_EquipmentComponent::DetachMeshFromPlayerHands() const {
	if (IsValid(RightHandHeldMeshComponent)) {
		RightHandHeldMeshComponent->SetStaticMesh(nullptr);
	}
}

UStaticMeshComponent* USatchel_EquipmentComponent::GetHeldItemStaticMeshComponent() const {
	return RightHandHeldMeshComponent;
}

void USatchel_EquipmentComponent::SetHeldItemStaticMeshComponent(UStaticMeshComponent* StaticMeshComponent) {
	RightHandHeldMeshComponent = StaticMeshComponent;
}

#pragma endregion Mesh

/////////
/// Equip/unequip
const USatchel_EquipmentBase* USatchel_EquipmentComponent::Equip(USatchel_EquipmentSlot* Slot, const USatchel_EquipmentBase* EquipmentItem) {

	if (!IsValid(EquipmentItem)) {
		UE_LOG(LogCharacterEquipment, Log, TEXT("Provided equipment was null; Did you mean to UnEquip?"))
		return nullptr;
	}

	USatchel_ItemFragment_Equippable* EquipFragment = EquipmentItem->FindFragment<USatchel_ItemFragment_Equippable>();
	if (!IsValid(EquipFragment)) {
		UE_LOG(LogCharacterEquipment, Warning, TEXT("Attempting to equip an item with no equip fragment, is this an error?"))
		return nullptr;
	}
	
	if (!IsValid(Slot)) {
		UE_LOG(LogCharacterEquipment, Log, TEXT("Could not equip item %s as slot did not exist"), *EquipmentItem->GetName());
		return nullptr;
	}
	if (GetOwner()->GetLocalRole() != ROLE_Authority) return nullptr;
	
	const USatchel_EquipmentBase* ExistingEquipment = nullptr;
	// We want to unequip before equipping to prevent stacking effects and abilities
	if (IsValid(Slot->PeekItem())) {
		ExistingEquipment = UnEquip(Slot);
		if (!IsValid(ExistingEquipment)) {
			UE_LOG(LogCharacterEquipment, Log, TEXT("Could not unequip existing item!"));
			return nullptr;
		}
	}
	
	if (!GrantGameplayAbilities(EquipmentItem, Slot) || !ApplyGameplayEffects(EquipmentItem, Slot)) {
		UE_LOG(LogCharacterEquipment, Log, TEXT("Could not apply equipment effects or abilities!"));
		return nullptr;
	}
	
	// Now we know we can remove the equipment - No more failing
	// We also don't need to grab the item from here, because we already have the unequipped item from the slot
	Slot->SetItem(EquipmentItem);
	EquipFragment->Equip(this);
	
	// Now we can equip and change our mesh
	// Skeletal mesh
	const USatchel_ItemFragment_SkeletalMesh* SkeletalMeshFragment =
		Cast<USatchel_ItemFragment_SkeletalMesh>(EquipmentItem->FindFragment(USatchel_ItemFragment_SkeletalMesh::StaticClass()));
	if (IsValid(SkeletalMeshFragment)) {
		if (USkeletalMesh* EquipmentMesh = SkeletalMeshFragment->Mesh; IsValid(EquipmentItem)) {
			if (USkeletalMeshComponent* SkelMesh = Cast<USkeletalMeshComponent>(Slot->EquipmentMeshComponent)) {
				SkelMesh->SetSkeletalMeshAsset(EquipmentMesh);
			}
		}
	}
	// Static mesh
	else {
		if (const USatchel_WeaponBase* WeaponBase = Cast<USatchel_WeaponBase>(EquipmentItem)) {
			const USatchel_ItemFragment_StaticMesh* StaticMeshFragment =
				Cast<USatchel_ItemFragment_StaticMesh>(EquipmentItem->FindFragment(USatchel_ItemFragment_StaticMesh::StaticClass()));
			if (IsValid(StaticMeshFragment)) {
				if (UStaticMesh* WeaponMesh = StaticMeshFragment->Mesh; IsValid(WeaponMesh)) {
					AttachMeshToPlayerHands(WeaponMesh, -WeaponBase->GetHandConnectionSocket()->RelativeLocation);
				}
			}
		}
	}

	if (IsValid(Slot->MeshToDeactivateOnEquip)) {
		Slot->MeshToDeactivateOnEquip->SetVisibility(false);
	}
	
	return ExistingEquipment;
}

const USatchel_EquipmentBase* USatchel_EquipmentComponent::UnEquip(USatchel_EquipmentSlot* Slot) {

	if (!IsValid(Slot)) return nullptr;
	
	if (!RevokeGameplayAbilities(Slot) || !RemoveGameplayEffects(Slot)) {
		UE_LOG(LogInventory, Warning, TEXT("Unable to unequip and remove gameplay abilities for slot %s"), *Slot->GetName())
		return nullptr;
	}
	const USatchel_EquipmentBase* ExistingEquipment = Cast<USatchel_EquipmentBase>(Slot->TakeItem());

	USatchel_ItemFragment_Equippable* EquipFragment = ExistingEquipment->FindFragment<USatchel_ItemFragment_Equippable>();
	if (IsValid(EquipFragment)) EquipFragment->UnEquip();
	
	const USatchel_ItemFragment_SkeletalMesh* SkeletalMeshFragment = Cast<USatchel_ItemFragment_SkeletalMesh>(ExistingEquipment->FindFragment(USatchel_ItemFragment_SkeletalMesh::StaticClass()));
	if (IsValid(SkeletalMeshFragment)) {
		if (USkeletalMeshComponent* SkeletalMesh = Cast<USkeletalMeshComponent>(Slot->EquipmentMeshComponent)) {
			SkeletalMesh->SetSkeletalMeshAsset(nullptr);
		}
	}
	else {
		if (UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(Slot->EquipmentMeshComponent)) {
			StaticMeshComponent->SetStaticMesh(nullptr);
			DetachMeshFromPlayerHands();
		}
	}

	if (IsValid(Slot->MeshToDeactivateOnEquip)) {
		Slot->MeshToDeactivateOnEquip->SetVisibility(true);
	}
	
	return ExistingEquipment;
}

//////////////
// Slots

bool USatchel_EquipmentComponent::AddEquipmentSlot(USatchel_EquipmentSlot* EquipmentSlot) {

	const int EquipmentIndex = EquipmentSlots.Add(EquipmentSlot);
	SetupEquipmentSlot(EquipmentSlot, EquipmentIndex);

	UE_LOG(LogCharacterEquipment, Log, TEXT("Equipment slot added for: %s"), *EquipmentSlot->AllowedItems.ToString());
	return true;
}

void USatchel_EquipmentComponent::SetupEquipmentSlot(USatchel_EquipmentSlot* EquipmentSlot, int Index) {
	// We should remove the equipment before adding the slot to make sure we don't have unequipped gear allocated

	const USatchel_EquipmentBase* StarterEquipment = nullptr;
	if (IsValid(EquipmentSlot->PeekItem())) {
		StarterEquipment = Cast<USatchel_EquipmentBase>(EquipmentSlot->TakeItem());
	}
	
	EquipmentSlot->Setup(Index, this, EquipmentSlot->AllowedItems);
	
	// If there was gear, now is when we equip it
	if (IsValid(StarterEquipment)) {
		Equip(EquipmentSlot, StarterEquipment);
	}
}

USatchel_EquipmentSlot* USatchel_EquipmentComponent::MakeEquipmentSlot(
	FGameplayTagContainer AllowedItems,
	USkeletalMeshComponent* EquipmentMeshComponent,
	USkeletalMeshComponent* MeshToDeactivateOnEquip,
	USatchel_EquipmentBase* BaseEquipment,
	const bool IsLocked) {
	return MakeEquipmentSlotGeneric(AllowedItems, EquipmentMeshComponent, MeshToDeactivateOnEquip, BaseEquipment, IsLocked);
}

USatchel_EquipmentSlot* USatchel_EquipmentComponent::MakeEquipmentSlotGeneric(
	FGameplayTagContainer AllowedItems,
	UMeshComponent* EquipmentMeshComponent,
	UMeshComponent* MeshToDeactivateOnEquip,
	USatchel_EquipmentBase* BaseEquipment,
	bool IsLocked) {
	
	USatchel_EquipmentSlot* EquipmentSlot = NewObject<USatchel_EquipmentSlot>();

	EquipmentSlot->ConstrainToType(USatchel_EquipmentBase::StaticClass());
	EquipmentSlot->AllowedItems = AllowedItems;
	EquipmentSlot->EquipmentMeshComponent = EquipmentMeshComponent;
	EquipmentSlot->MeshToDeactivateOnEquip = MeshToDeactivateOnEquip;
	EquipmentSlot->bIsLocked = IsLocked;

	// We know the slot is empty, we just created it!
	EquipmentSlot->SetItem(BaseEquipment);

	return EquipmentSlot;
}

////////////
/// Apply effects/abilities
#pragma region AbilitiesAndEffects

bool USatchel_EquipmentComponent::ApplyGameplayEffects(const USatchel_ItemBase* Item, USatchel_EquipmentSlot* EquipmentSlot) const {
	if (!IsValid(AbilitySystemComponent)) return false;

	ensure(IsValid(Item));
	
	for (auto GE : Item->GameplayEffects) {
		if (IsValid(GE)) {
			FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
			EffectContextHandle.AddSourceObject(this);

			if (FGameplayEffectSpecHandle GameplayEffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GE, 1, EffectContextHandle);
				GameplayEffectSpecHandle.IsValid())
			{
				EquipmentSlot->ActiveEffectHandles.Add(AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*GameplayEffectSpecHandle.Data.Get(), AbilitySystemComponent));
			}
		}
	}
	return true;
}

bool USatchel_EquipmentComponent::RemoveGameplayEffects(USatchel_EquipmentSlot* EquipmentSlot) const {

	if (!IsValid(AbilitySystemComponent)) return false;
	
	for (auto ActiveEffect : EquipmentSlot->ActiveEffectHandles) {
		if (ActiveEffect.IsValid())
		{
			AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveEffect);
		}
	}
	EquipmentSlot->ActiveEffectHandles.Empty();
	return true;
}

bool USatchel_EquipmentComponent::GrantGameplayAbilities(const USatchel_ItemBase* Item, USatchel_EquipmentSlot* EquipmentSlot) const {

	if (!IsValid(AbilitySystemComponent)) return false;
	ensure(IsValid(Item));
	
	for (auto Ability : Item->GameplayAbilities) {
		if (IsValid(Ability)) {
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability);
			EquipmentSlot->ActiveAbilityHandles.Add(AbilitySystemComponent->GiveAbility(AbilitySpec));
		}
	}
	return true;
}

bool USatchel_EquipmentComponent::RevokeGameplayAbilities(USatchel_EquipmentSlot* EquipmentSlot) const {
	if (!IsValid(AbilitySystemComponent)) return false;
	
	for (auto ActiveAbility : EquipmentSlot->ActiveAbilityHandles) {
		if(ActiveAbility.IsValid()) {
			AbilitySystemComponent->ClearAbility(ActiveAbility);
		}
	}
	EquipmentSlot->ActiveAbilityHandles.Empty();
	return true;
}

#pragma endregion AbilitiesAndEffects
