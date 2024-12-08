// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Highlight/Satchel_HighlightableComponent.h"


// Sets default values for this component's properties
USatchel_HighlightableComponent::USatchel_HighlightableComponent() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void USatchel_HighlightableComponent::BeginPlay() {
	Super::BeginPlay();
	
	TSet<UActorComponent*> OwnerComponents = GetOwner()->GetComponents();
	for (const auto OwnerComponent : OwnerComponents) {
		if (UMeshComponent* MeshComp = Cast<UMeshComponent>(OwnerComponent)) {
			AddComponentToHighlight(MeshComp);
		}
	}
}

void USatchel_HighlightableComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// If highlighted, we want to attempt to clean and remove the highlight
	if (!CurrentlyHighlighted) return;

	//We only want to clean if a minimum number of ticks has passed 
	if (CurrentTicksSinceHighlight >= TicksBeforeCleaning) {
		for (int i = 0; i < ComponentsToHighlight.Num(); i++) {
			UPrimitiveComponent* PrimitiveComponent = ComponentsToHighlight.Get(FSetElementId::FromInteger(i));
			PrimitiveComponent->SetCustomDepthStencilValue(BaseCustomDepth[i]);
		}
		CurrentlyHighlighted = false;
		return;
	}

	// Only count up ticks if highlighted. Prevents int overflow.
	CurrentTicksSinceHighlight++;

}


void USatchel_HighlightableComponent::Highlight() {
	HighlightColorOverride(DefaultHighlightColor);
}

void USatchel_HighlightableComponent::HighlightColorOverride(EC_HighlightColor HighlightColor) {
	// If not highlighted, highlight. Otherwise, refresh highlight 
	if (!CurrentlyHighlighted) {
		for (int i = 0; i < ComponentsToHighlight.Num(); i++) {
			UPrimitiveComponent* PrimitiveComponent = ComponentsToHighlight.Get(FSetElementId::FromInteger(i));
			PrimitiveComponent->SetCustomDepthStencilValue(HighlightColorMap[HighlightColor]);
		}
	}

	CurrentTicksSinceHighlight = 0;
	CurrentlyHighlighted = true;
}

void USatchel_HighlightableComponent::AddComponentToHighlight(UPrimitiveComponent* Component) {
	const int Index = ComponentsToHighlight.Add(Component).AsInteger();
	
	UPrimitiveComponent* PrimitiveComponent = Component;
	PrimitiveComponent->SetRenderCustomDepth(true);
	BaseCustomDepth.EmplaceAt(Index, PrimitiveComponent->CustomDepthStencilValue);
}

void USatchel_HighlightableComponent::RemoveComponentToHighlight(UPrimitiveComponent* Component) {
	const int Index = ComponentsToHighlight.FindId(Component).AsInteger();
	ComponentsToHighlight.Remove(Component);

	if (BaseCustomDepth.Num() >= Index) {
		Component->SetCustomDepthStencilValue(BaseCustomDepth[Index]);
	}
	
}



