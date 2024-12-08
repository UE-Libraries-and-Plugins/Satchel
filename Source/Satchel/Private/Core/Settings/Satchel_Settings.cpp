// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Settings/Satchel_Settings.h"

#include "Blueprint/UserWidget.h"
#include "Core/Satchel_ItemInstance.h"

TSubclassOf<ASatchel_ItemInstance> USatchel_Settings::GetLoadedItemInstanceTemplate(bool ForceReload) const {
	TSubclassOf<ASatchel_ItemInstance> Instance = ItemInstanceTemplate.Get();
	if (!IsValid(Instance)) {
		Instance = ItemInstanceTemplate.LoadSynchronous();
	}

	return Instance;
}

TSubclassOf<UUserWidget> USatchel_Settings::GetLoadedItemChangePopupTemplate(bool ForceReload) const {
	TSubclassOf<UUserWidget> Instance = ItemChangePopupTemplate.Get();
	if (!IsValid(Instance)) {
		Instance = ItemChangePopupTemplate.LoadSynchronous();
	}

	return Instance;
}
