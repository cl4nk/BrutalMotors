// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Life/LivingPlayerState.h"
#include "BMCarMeta.h"
#include "BMPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class BRUTALMOTORS_API ABMPlayerState : public ALivingPlayerState
{
	GENERATED_BODY()
	
public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	UPROPERTY(Transient, Replicated)
	FBMArchetypeItem Wheel;

	UPROPERTY(Transient, Replicated)
	FBMArchetypeItem Frame;

	UPROPERTY(Transient, Replicated)
	FBMArchetypeItem PrimaryShoot;

	UPROPERTY(Transient, Replicated)
	FBMArchetypeItem SecondaryShoot;

	virtual void OverrideWith(APlayerState* PlayerState) override;
	virtual void CopyProperties(APlayerState* PlayerState) override;
};
