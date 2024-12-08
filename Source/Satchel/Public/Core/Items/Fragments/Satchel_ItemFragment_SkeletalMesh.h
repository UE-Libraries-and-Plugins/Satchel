// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Satchel_ItemFragment_Base.h"
#include "Satchel_ItemFragment_SkeletalMesh.generated.h"

/**
 * 
 */
UCLASS()
class SATCHEL_API USatchel_ItemFragment_SkeletalMesh : public USatchel_ItemFragment_Base {
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "SkeletalMesh", BlueprintReadOnly)
	USkeletalMesh* Mesh;
};
