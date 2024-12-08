// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Highlight/Satchel_EntityHighlightSystem.h"

#include "Core/Highlight/Satchel_HighlightableComponent.h"
#include "GameFramework/PlayerState.h"


// Sets default values
ASatchel_EntityHighlightSystem::ASatchel_EntityHighlightSystem() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASatchel_EntityHighlightSystem::BeginPlay() {
	Super::BeginPlay();

	PlayerController = GetWorld()->GetFirstPlayerController();
}

// Called every frame
void ASatchel_EntityHighlightSystem::Tick(float DeltaTime) {
	// local only
	if (!HasAuthority()) return;
	Super::Tick(DeltaTime);

	FHitResult Hit;
	if (PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(HighlightChannel), false, Hit)) {
		const AActor* HitActor = Hit.GetActor();
		
		if (IsValid(HitActor)) {
			if (USatchel_HighlightableComponent* HighlightableComponent = HitActor->FindComponentByClass<USatchel_HighlightableComponent>(); IsValid(HighlightableComponent)) {
				HighlightableComponent->Highlight();
			}
		}
	}
}

