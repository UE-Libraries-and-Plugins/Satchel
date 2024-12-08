// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Fragments/Satchel_ItemFragment_Base.h"
#include "Interface/Context_Giver.h"
#include "UObject/Object.h"
#include "Satchel_ItemBase.generated.h"

class USatchel_ItemSlot;
class USatchel_ItemFragment_Base;
class UGameplayAbility;
class UGameplayEffect;
class UC_GameplayAbility;
class UC_GameplayEffect;

DEFINE_LOG_CATEGORY_STATIC(LogSatchelItem, Log, All);

/**
 * This isn't great, and in fact, has an ick factor.
 *
 * We should make this better in the future by adding this ID to a global list
 * as well as validating that this ID is not used anywhere
 * That way we can globally refer to items by their ID without holding a reference to the actual item
 * and instead loading the UDataAsset on request
 **/
USTRUCT(BlueprintType)
struct FC_ItemID {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag Identifier;

	bool operator==(const FC_ItemID& Other) const {
		return Other.Identifier == Identifier;
	}
};

/**
 * Base class for all items
 *
 * Provides helper functions and default effects and abilities all items may require
 * Does NOT enforce usage however, and leaves the implementation up to the child
 **/
UCLASS(Abstract, Blueprintable, BlueprintType, Const)
class SATCHEL_API USatchel_ItemBase : public UObject, public IContext_Giver {
	GENERATED_BODY()
	
	/**
	 * The owner of this item
	 * May be null if the item is unowned, of the owner has died
	 */
	UPROPERTY(VisibleAnywhere)
	AActor* Owner;

	UPROPERTY(EditAnywhere, Instanced)
	TArray<TObjectPtr<USatchel_ItemFragment_Base>> Fragments; 
	
// Properties
public:
	
	/// The Item ID is incredibly important, and is used to define what an item is, how it's filtered, if it's valid
	/// for a specific item slot, and what type of item it is. 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Settings")
	FGameplayTag ItemId;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category =  "Mesh")
	UStaticMesh* StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category =  "Item Settings")
	UTexture2D* Icon;
	
	/**
	 * The display name that can be used to refer to the item, or print its name in UI 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly,  Category = "Item Settings")
	FText DisplayName;

	/**
	 * User friendly tooltip to describe the item in UI
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Settings")
	FText Description; 

	/**
	 * The effects that using this item cause.
	 * Removed once the item is no longer in use
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Modifiers")
	TArray<TSubclassOf<UGameplayEffect>> GameplayEffects;

	/**
	 * Abilities that are granted to the owning actor on use, and revokes when the item is unused
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Modifiers")
	TArray<TSubclassOf<UGameplayAbility>> GameplayAbilities;

	/**
	 * Context entries to pass down when interacting with this item
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Context|Entries")
	TSet<UContext_ActionEntry*> ContextEntries;

	/**
	 * Primary entry that will be used when activating this item (Use, Equip, Open, etc)
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Context|Entries")
	UContext_ActionEntry* PrimaryContextEntry;

// Functions
public:

	virtual void GetGiverContextEntries_Implementation(TSet<UContext_ActionEntry*>& OutContextEntries) override;
	
	/**
	 * Sets the owner of the item. This is mandatory 
	 * @param Instigator The owner of this object
	 */
	UFUNCTION(BlueprintCallable)
	virtual void SetOwner(AActor* Instigator) {
		Owner = Instigator;
	};

	/**
	 * Attempts to return the owner of the object.
	 * This may be null, and its validity should be checked.
	 * The owner being null is expected if the item is not currently owned, or is orphaned
	 * @return A pointer to the owner, or null
	 */
	UFUNCTION(BlueprintCallable)
	virtual AActor* GetOwner() const {
		return Owner;
	}

	/**
	 * Attempts to return the static mesh of the item.
	 * In case that this is null, it notifies the user.
	 * @return The static mesh of the item, or null
	 */
	UFUNCTION(BlueprintCallable)
	virtual UStaticMesh* GetStaticMesh() const
	{
		if (ensureAlways(StaticMesh))
			return StaticMesh;
		
		return nullptr;
	}

	/**
	 * Find a fragment kept on the item.
	 * A fragment is an extension of the item, or something that may not exist on all item instances. 
	 * @param FragmentClass Class to search for
	 * @return Fragment, or nullptr
	 */
	UFUNCTION(BlueprintCallable)
	USatchel_ItemFragment_Base* FindFragment(TSubclassOf<USatchel_ItemFragment_Base> FragmentClass) const;

	/// Generic template for finding fragments in C++
	template<class TFragment>
	TFragment* FindFragment() const;
	
	UFUNCTION(BlueprintCallable)
	bool FragmentsCanAccept(const USatchel_ItemSlot* Slot) const {
		for (const auto Fragment : Fragments) {
			if (!Fragment->OnCanAccept(this, Slot)) return false;
		}
		return true;
	};
	
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif

};

template <class TFragment>
TFragment* USatchel_ItemBase::FindFragment() const {
	UClass* FragmentClass = TFragment::StaticClass();
	USatchel_ItemFragment_Base* BaseFragment = FindFragment(FragmentClass);
	TFragment* CastFragment = Cast<TFragment>(BaseFragment);

	if (!IsValid(CastFragment)) {
		UE_LOG(LogSatchelItem, Log, TEXT("Could not find fragment of type %s for item %s"),
			*FragmentClass->GetName(),
			*DisplayName.ToString())
	}

	return CastFragment;
}
