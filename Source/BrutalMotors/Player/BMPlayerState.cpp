// Fill out your copyright notice in the Description page of Project Settings.

#include "BMPlayerState.h"
#include "UnrealNetwork.h"

void ABMPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Maybe use RPC functions instead of replicating
	DOREPLIFETIME(ABMPlayerState, Wheel);
	DOREPLIFETIME(ABMPlayerState, Frame);
	DOREPLIFETIME(ABMPlayerState, PrimaryShoot);
	DOREPLIFETIME(ABMPlayerState, SecondaryShoot);
}

void ABMPlayerState::OverrideWith(APlayerState * PlayerState)
{
	Super::OverrideWith(PlayerState);

	ABMPlayerState const * const BMPlayerState = Cast<ABMPlayerState>(PlayerState);
	if (BMPlayerState)
	{
		Wheel = BMPlayerState->Wheel;
		Frame = BMPlayerState->Frame;
		PrimaryShoot = BMPlayerState->PrimaryShoot;
		SecondaryShoot = BMPlayerState->SecondaryShoot;
	}
}

void ABMPlayerState::CopyProperties(APlayerState * PlayerState)
{
	Super::CopyProperties(PlayerState);

	ABMPlayerState * BMPlayerState = Cast<ABMPlayerState>(PlayerState);
	if (BMPlayerState)
	{
		BMPlayerState->Wheel = Wheel;
		BMPlayerState->Frame = Frame;
		BMPlayerState->PrimaryShoot = PrimaryShoot;
		BMPlayerState->SecondaryShoot = SecondaryShoot;
	}
}