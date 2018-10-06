// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LivingPlayerController.generated.h"

class ALivingPlayerState;

/**
 * 
 */
UCLASS(ClassGroup = (Life))
class BRUTALMOTORS_API ALivingPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	virtual void Possess(APawn * Pawn) override;
	virtual void UnPossess() override;

	ALivingPlayerState * GetLivingPlayerState() const;
	
protected:

	virtual void BeginPlay() override;

	void OnLifeChanged(int32 Life, float LifePercent);
	void OnDeathChanged(bool bIsAlive);

private:

	TWeakObjectPtr<ALivingPlayerState> LivingPlayerState;
};
