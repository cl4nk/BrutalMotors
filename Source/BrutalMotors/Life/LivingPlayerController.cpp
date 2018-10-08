// Fill out your copyright notice in the Description page of Project Settings.

#include "LivingPlayerController.h"
#include "GameFramework/Pawn.h"
#include "LifeComponent.h"
#include "LivingPlayerState.h"

void ALivingPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Maybe set that in the getter if the player state do not exists here
	LivingPlayerState = Cast<ALivingPlayerState>(PlayerState);
}

void ALivingPlayerController::Possess(APawn* Pawn)
{
	Super::Possess(Pawn);

	if (!IsLocalController())
	{
		if (ULifeComponent* LifeComponent = Pawn->FindComponentByClass<ULifeComponent>())
		{
			LifeComponent->LifeChanged.AddDynamic(this, &ALivingPlayerController::OnLifeChanged);
			LifeComponent->DeathChanged.AddDynamic(this, &ALivingPlayerController::OnDeathChanged);

			if (ALivingPlayerState* LivingPlayerState = GetLivingPlayerState())
			{
				LivingPlayerState->Health = LifeComponent->GetCurrentLife();
			}
		}
	}
}

void ALivingPlayerController::UnPossess()
{
	APawn const* const Pawn = GetPawn();
	if (Pawn && !IsLocalController())
	{
		if (ULifeComponent* LifeComponent = Pawn->FindComponentByClass<ULifeComponent>())
		{
			LifeComponent->LifeChanged.RemoveAll(this);
			LifeComponent->DeathChanged.RemoveAll(this);
		}
	}

	Super::UnPossess();
}

ALivingPlayerState* ALivingPlayerController::GetLivingPlayerState() const
{
	return LivingPlayerState.Get();
}

void ALivingPlayerController::OnLifeChanged(int32 const Life, float const LifePercent)
{
	ALivingPlayerState* LivingPlayerState = GetLivingPlayerState();
	if (LivingPlayerState)
	{
		LivingPlayerState->Health = Life;
	}
}

void ALivingPlayerController::OnDeathChanged(bool const bIsAlive)
{
	APawn* Pawn = GetPawn();
	if (Pawn)
	{
		if (bIsAlive)
		{
			Pawn->EnableInput(this);
		}
		else
		{
			Pawn->DisableInput(this);
		}
	}
}
