// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Satchel_HighlightableComponent.generated.h"

UENUM()
enum EC_HighlightColor {
	NONE	= 0,
	Black	= 1,
	Red		= 2,
	Blue	= 3,
	Purple	= 4,
	Green	= 5,
	Yellow	= 6
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SATCHEL_API USatchel_HighlightableComponent : public UActorComponent {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USatchel_HighlightableComponent();

protected:
	//These values are the stencil depth value that equates to the color
	UPROPERTY()
	TMap<TEnumAsByte<EC_HighlightColor>, int> HighlightColorMap = {
		{NONE, 999},
		{Black, 0},
		{Red, 10},
		{Blue, 11},
		{Purple, 12},
		{Green, 13},
		{Yellow, 14},
	};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TEnumAsByte<EC_HighlightColor> DefaultHighlightColor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TSet<UPrimitiveComponent*> ComponentsToHighlight;
	
	virtual void BeginPlay() override;

private:

	UPROPERTY()
	TArray<int> BaseCustomDepth;

	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess))
	bool CurrentlyHighlighted;

	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess, ClampMin = 1))
	int TicksBeforeCleaning = 1;

	UPROPERTY()
	int CurrentTicksSinceHighlight = 0;
	
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * Highlight the object for this frame
	 */
	UFUNCTION()
	void Highlight();

	/**
	 * Highlight the object for this with a specific color, ignoring the preset color
	 */
	UFUNCTION()
	void HighlightColorOverride(EC_HighlightColor HighlightColor);

	UFUNCTION(BlueprintCallable)
	void AddComponentToHighlight(UPrimitiveComponent* Component);

	UFUNCTION(BlueprintCallable)
	void RemoveComponentToHighlight(UPrimitiveComponent* Component);
};
