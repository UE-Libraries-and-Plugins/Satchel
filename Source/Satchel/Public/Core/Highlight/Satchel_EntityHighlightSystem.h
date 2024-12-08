// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Satchel_EntityHighlightSystem.generated.h"

UCLASS()
class SATCHEL_API ASatchel_EntityHighlightSystem : public AActor {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = true))
	TEnumAsByte<ECollisionChannel> HighlightChannel = ECC_Visibility;

	UPROPERTY(VisibleAnywhere)
	APlayerController* PlayerController;
	
public:
	// Sets default values for this actor's properties
	ASatchel_EntityHighlightSystem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
