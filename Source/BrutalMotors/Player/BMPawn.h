// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "BMPawn.generated.h"

UCLASS()
class BRUTALMOTORS_API ABMPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABMPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Parameters|FOV")
	class UCurveFloat const * FovBySpeedCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Parameters|FOV", meta = (ClampMin = 0.0f))
	float FOVStiffness;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Parameters|Aim")
	FVector TargetPointInPawnSpace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Parameters|Aim", meta = (ClampMin = 0.0f))
	float MaxAimRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Parameters|Aim", meta = (ClampMin = 0.0f))
	float AimOffsetStiffness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Parameters", meta = (ClampMin = 0.0f))
	float CameraDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Parameters")
	FRotator CameraRotation = FRotator(260.0f, 0.0f, 0.0f);

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	FTransform GetMuzzleTransform() const;
	virtual FTransform GetMuzzleTransform_Implementation() const;

	UFUNCTION(BlueprintPure)
	FVector GetAimDirection() const;

	FVector2D GetUnsafeAimDirection2D() const;
	FVector2D GetAimDirection2D() const;

protected:
	virtual void CalcCamera(float DeltaTime, struct FMinimalViewInfo & OutResult) override;

	void OnShootPressed();
	void OnShootReleased();

	UFUNCTION(Client, reliable, BlueprintCallable)
	void Client_ShootProjectile();
	virtual void Client_ShootProjectile_Implementation();

	UFUNCTION(Server, reliable, WithValidation)
	void Server_ShootProjectile();
	virtual bool Server_ShootProjectile_Validate();
	virtual void Server_ShootProjectile_Implementation();

	UFUNCTION(NetMulticast, unreliable)
	void NetMulticast_OnProjectileShot(FTransform const & ProjectileTransform);
	virtual void NetMulticast_OnProjectileShot_Implementation(FTransform const & ProjectileTransform);

	UFUNCTION(BlueprintImplementableEvent)
	void OnProjectileShot(FTransform const & ProjectileTransform);

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ProjectileToSpawn;

// Camera state
	FVectorSpringState AimOffsetSpringState;
	FFloatSpringState FOVSpringState;
	FVector AimOffset;

// Aim direction
	FVector2D AimDirectionUnsafe2D;
	FVector2D AimDirection2D;
	FVector AimDirection;
};
