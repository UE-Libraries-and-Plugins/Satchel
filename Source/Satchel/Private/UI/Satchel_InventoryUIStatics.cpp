// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Satchel_InventoryUIStatics.h"

#include "Core/Settings/Satchel_Settings.h"
#include "GameFramework/PlayerState.h"
#include "Helpers/UMGC_EntityUIStatics.h"
#include "UMG/UMGC_FloatingOverheadText.h"

void USatchel_InventoryUIStatics::SpawnItemInventoryPopup(const FItemPopupPayload& Item, const AActor* FollowActor) {
	const FString Text = Item.bWasRemoved ?
							 FString::Printf(TEXT("--  %ls"), *Item.PopupName.ToString()) :
							 FString::Printf(TEXT("++  %ls"), *Item.PopupName.ToString());

	// Owner might be player state, since inventory should exist on player state for players
	const APlayerState* PlayerState = Cast<APlayerState>(FollowActor);
	if (IsValid(PlayerState)) {
		FollowActor = PlayerState->GetPawn();
	}

	const USatchel_Settings* Settings = GetDefault<USatchel_Settings>();
	UUMGC_FloatingOverheadText* ItemChangePopup = CreateWidget<UUMGC_FloatingOverheadText>(FollowActor->GetWorld(), Settings->GetLoadedItemChangePopupTemplate());
	ItemChangePopup->SetWorldLocation(UUMGC_EntityUIStatics::GetActorOverheadLocation(FollowActor));
	ItemChangePopup->SetText(FText::FromString(Text));
	ItemChangePopup->SetIcon(Item.Icon);
	ItemChangePopup->AddToViewport();
}
