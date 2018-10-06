// Fill out your copyright notice in the Description page of Project Settings.

#include "BMPawn.h"
#include "Camera/CameraTypes.h"
#include "Curves/CurveFloat.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"

// Sets default values
ABMPawn::ABMPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABMPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABMPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Stock axis value to prevent multiple GetInputAxisValue and GetSafeNormal in the same frame 
	AimDirectionUnsafe2D = FVector2D(GetInputAxisValue("LookRight"), GetInputAxisValue("LookForward"));
	AimDirection2D = AimDirectionUnsafe2D.GetSafeNormal();
	AimDirection = FVector(AimDirection2D.X, AimDirection2D.Y, 0.0f);
}

// Called to bind functionality to input
void ABMPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("LookForward");
	PlayerInputComponent->BindAxis("LookRight");

	PlayerInputComponent->BindAction("Shoot", EInputEvent::IE_Pressed, this, &ABMPawn::OnShootPressed);
	PlayerInputComponent->BindAction("Shoot", EInputEvent::IE_Released, this, &ABMPawn::OnShootReleased);
}

FTransform ABMPawn::GetMuzzleTransform_Implementation() const
{
	FTransform MuzzleTransform = GetTransform();
	MuzzleTransform.SetRotation(GetAimDirection().ToOrientationQuat());
	return MuzzleTransform;
}

FVector2D ABMPawn::GetUnsafeAimDirection2D() const
{
	return AimDirectionUnsafe2D;
}

FVector2D ABMPawn::GetAimDirection2D() const
{
	return AimDirection2D;
}

FVector ABMPawn::GetAimDirection() const
{
	return AimDirection;
}

void ABMPawn::CalcCamera(float DeltaTime, FMinimalViewInfo & OutResult)
{
	Super::CalcCamera(DeltaTime, OutResult);

	OutResult.Rotation = CameraRotation;

	// Add an offset to adjust the target point with the aim direction
	float const AimRatio = GetUnsafeAimDirection2D().Size();
	float const AimRadius = FMath::Lerp(0.0f, MaxAimRadius, FMath::Min(1.0f, AimRatio));

	FVector const NewAimOffset = GetAimDirection() * AimRadius;
	AimOffset = UKismetMathLibrary::VectorSpringInterp(AimOffset, NewAimOffset, AimOffsetSpringState, AimOffsetStiffness, 1.0f, DeltaTime, 1.0f);

	FVector const CameraTargetPosition = GetActorTransform().TransformPosition(TargetPointInPawnSpace) + CameraRotation.Vector() * CameraDistance * -1.0f + AimOffset;

	OutResult.Location = CameraTargetPosition;

	// Adjust FOV to simulate a zoom
	// Maybe use ensure if we want always want a curve
	if (FovBySpeedCurve)
	{
		float const TargetFov = FovBySpeedCurve->GetFloatValue(GetVelocity().Size());
		//OutResult.FOV = FMath::Lerp(OutResult.FOV, TargetFov, DeltaTime * 10.0f);
		OutResult.FOV = UKismetMathLibrary::FloatSpringInterp(OutResult.FOV, TargetFov, FOVSpringState, FOVStiffness, 1.0f, DeltaTime, 1.0f);
	}
}

void ABMPawn::OnShootPressed()
{
	Client_ShootProjectile();
}

void ABMPawn::OnShootReleased()
{
}

void ABMPawn::Client_ShootProjectile_Implementation()
{
	if (Role == ROLE_SimulatedProxy)
	{
		Server_ShootProjectile();
	}
}

void ABMPawn::Server_ShootProjectile_Implementation()
{
	if (Role == ROLE_Authority)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		SpawnParameters.Instigator = this;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		FTransform const MuzzleTransform = GetMuzzleTransform();
		GetWorld()->SpawnActorAbsolute(ProjectileToSpawn.Get(), MuzzleTransform, SpawnParameters);

		NetMulticast_OnProjectileShot(MuzzleTransform);
	}
}

bool ABMPawn::Server_ShootProjectile_Validate()
{
	return true;
}

void ABMPawn::NetMulticast_OnProjectileShot_Implementation(FTransform const & ProjectileTransform)
{
	
}
