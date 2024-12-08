// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Satchel_Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class SATCHEL_API USatchel_Interactable : public UInterface {
	GENERATED_BODY()
};

/**
 * 
 */
class SATCHEL_API ISatchel_Interactable {
	GENERATED_BODY()

public:
	/**
	 * Event occurs once 
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact|Event")
	void EnterInteractionZone(AActor* ActorEnteringZone);

	/**
	 * 
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact|Event")
	void ExitInteractionZone(AActor* ActorExitingZone);

	/**
	 * 
	 * @return 
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact|Validate")
	bool CanInteract(AActor* ActorInteracting);

	/**
	 * 
	 * @return 
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact|Action")
	bool Interact(AActor* ActorInteracting);
	
};
