// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Items/Resource/Satchel_ResourceGeneratorComponent.h"


// Sets default values for this component's properties
USatchel_ResourceGeneratorComponent::USatchel_ResourceGeneratorComponent() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USatchel_ResourceGeneratorComponent::BeginPlay() {
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USatchel_ResourceGeneratorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                        FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

