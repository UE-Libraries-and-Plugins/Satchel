// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Satchel_Interactable.h"
#include "GameFramework/Actor.h"
#include "Interface/Context_Holder.h"
#include "Satchel_ItemInstance.generated.h"

class UContext_HolderComponent;
class ISatchel_InventoryHolder;
class UCharacterMovementComponent;
class UBoxComponent;
class USatchel_ItemBase;

/**
 * An actor instance representing a singular item within the world
 *
 * The purpose of this actor is to show and represent item data to the player, and allow entities to interact
 * with the item
 */
UCLASS()
class SATCHEL_API ASatchel_ItemInstance : public AActor, public ISatchel_Interactable {
	GENERATED_BODY()
	
	/**
	 * If you can interact with and grab this item.
	 * Locked items exist in the world, but cannot be interacted with until they are unlocked
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn, AllowPrivateAccess = true))
	bool bLockItem;
	
	/**
	 * The data that this instance represents - Should not be changed or modified after creation
	 */
	UPROPERTY(VisibleAnywhere)
	const USatchel_ItemBase* ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn, AllowPrivateAccess = true))
	bool DestroyOnPickup;

	UPROPERTY()
	bool bCanInteract = false;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn, AllowPrivateAccess = true))
	TSubclassOf<USatchel_ItemBase> ItemTemplate;
	
protected:
	// Components

	UPROPERTY(EditDefaultsOnly)
	UContext_HolderComponent* ContextComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBoxComponent* BoxCollider;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* InteractionCollider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UCharacterMovementComponent* MovementComponent;
	
public:
	UFUNCTION()
	void InteractionBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg,
	                             const FHitResult& HitResult);

	UFUNCTION()
	void InteractionEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I);
	// Sets default values for this actor's properties
	ASatchel_ItemInstance();

	/// Attempts to pickup this item and provide it to the passed actor.
	/// This will only succeed if the actor implements the InventoryHolder interface, and the inventory accepts the item.
	/// @param InstigatingActor Actor to provide the item to
	/// @return If the item was successfully added
	UFUNCTION(BlueprintCallable, Category="Satchel|Item")
	bool TryPickUp(AActor* InstigatingActor);
	
	/**
	 * Locks the item so it cannot be interacted with
	 */
	void LockItem() {
		bLockItem = true;
	}

	/**
	 * Unlocks the item so it can be interacted with
	 */
	void UnlockItem() {
		bLockItem = false;
	}
	
	/**
	 * Retrieve the data that this item instance represents and holds
	 */
	UFUNCTION(BlueprintCallable)
	const USatchel_ItemBase* GetItemData() const {
		return ItemData;
	};

	UFUNCTION(BlueprintCallable)
	void SetItemData(TSubclassOf<USatchel_ItemBase> InstanceTemplate);

	UFUNCTION(BlueprintCallable)
	void AddImpulse(FVector Impulse, bool VelChange) const;
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void StartDestroy();

public:

	// Interactable interface
	virtual bool Interact_Implementation(AActor* ActorInteracting) override;
	virtual bool CanInteract_Implementation(AActor* ActorInteracting) override;
	virtual void EnterInteractionZone_Implementation(AActor* ActorEnteringZone) override;
	virtual void ExitInteractionZone_Implementation(AActor* ActorExitingZone) override;

};
