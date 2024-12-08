// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/Items/Equipment/Weapons/Satchel_WeaponBase.h"
#include "Engine/StaticMeshSocket.h"

USatchel_WeaponBase::USatchel_WeaponBase() {
}

UStaticMeshSocket* USatchel_WeaponBase::GetHandConnectionSocket() const {
	if (IsValid(StaticMesh)) {
		TArray<UStaticMeshSocket*> Sockets = StaticMesh->GetSocketsByTag(HandConnectSocketName);

		for (UStaticMeshSocket* Socket : Sockets) {
			return Socket;
		}
	}

	return nullptr;
}
