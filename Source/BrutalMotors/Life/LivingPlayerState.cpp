// Fill out your copyright notice in the Description page of Project Settings.

#include "LivingPlayerState.h"
#include "UnrealNetwork.h"

void ALivingPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALivingPlayerState, Health);
}

void ALivingPlayerState::OverrideWith(APlayerState * PlayerState)
{
	Super::OverrideWith(PlayerState);

	ALivingPlayerState const * LivingPlayerState = Cast<ALivingPlayerState>(PlayerState);
	if (LivingPlayerState)
	{
		Health = LivingPlayerState->Health;
	}
}

void ALivingPlayerState::CopyProperties(APlayerState * PlayerState)
{
	Super::CopyProperties(PlayerState);

	ALivingPlayerState * LivingPlayerState = Cast<ALivingPlayerState>(PlayerState);
	if (LivingPlayerState)
	{
		LivingPlayerState->Health = Health;
	}
}
