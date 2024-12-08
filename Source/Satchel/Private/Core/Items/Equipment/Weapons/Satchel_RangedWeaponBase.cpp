// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Items/Equipment/Weapons/Satchel_RangedWeaponBase.h"

const AActor* USatchel_RangedWeaponBase::GetBaseProjectile() const {
	return BaseProjectile.GetDefaultObject();
}

UStaticMeshSocket* USatchel_RangedWeaponBase::GetProjectileSpawnSocket() {
	if (IsValid(StaticMesh)) {
		for (TArray<UStaticMeshSocket*> Sockets = StaticMesh->GetSocketsByTag(ProjectileSpawnSocketName);
			UStaticMeshSocket* Socket : Sockets) {
			return Socket;
		}
	}

	return nullptr;
}
