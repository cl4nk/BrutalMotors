// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LivingPlayerState.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Life))
class BRUTALMOTORS_API ALivingPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Transient, Replicated)
	int32 Health;

protected:

	virtual void OverrideWith(APlayerState* PlayerState) override;
	virtual void CopyProperties(APlayerState* PlayerState) override;
};
