// Fill out your copyright notice in the Description page of Project Settings.

#include "BMPlayerState.h"
#include "UnrealNetwork.h"

void ABMPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Maybe use RPC functions instead of replicating
	DOREPLIFETIME(ABMPlayerState, CarConfig);
}

void ABMPlayerState::OverrideWith(APlayerState * PlayerState)
{
	Super::OverrideWith(PlayerState);

	ABMPlayerState const * const BMPlayerState = Cast<ABMPlayerState>(PlayerState);
	if (BMPlayerState)
	{
		CarConfig = BMPlayerState->CarConfig;
	}
}

void ABMPlayerState::CopyProperties(APlayerState * PlayerState)
{
	Super::CopyProperties(PlayerState);

	ABMPlayerState * BMPlayerState = Cast<ABMPlayerState>(PlayerState);
	if (BMPlayerState)
	{
		BMPlayerState->CarConfig = CarConfig;
	}
}