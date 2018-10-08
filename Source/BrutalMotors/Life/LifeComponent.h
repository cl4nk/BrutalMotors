// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LifeComponent.generated.h"

UCLASS( ClassGroup=(Life), meta=(BlueprintSpawnableComponent) )
class BRUTALMOTORS_API ULifeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULifeComponent();

	UFUNCTION(BlueprintCallable)
	void AddLifePercent(float const LifePercentToAdd, bool bEvenIfDead);

	UFUNCTION(BlueprintCallable)
	void AddLife(int32 const LifeToAdd, bool bEvenIfDead);

	UFUNCTION(BlueprintPure)
	int32 GetMaxLife() const;

	UFUNCTION(BlueprintPure)
	int32 GetCurrentLife() const;

	UFUNCTION(BlueprintPure)
	float GetCurrentLifePercent() const;

	UFUNCTION(BlueprintCallable)
	void Reset();

	UFUNCTION(BlueprintPure)
	bool IsAlive() const;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDeathChanged, bool, bIsAlive);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLifeChanged, int32, Life, float, LifePercent);
	FDeathChanged DeathChanged;
	FLifeChanged LifeChanged;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type const EndPlayReason) override;

	void OnTakeDamage(class AActor* DamagedActor, float Damage, class UDamageType const* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	bool IsAllowedToModifyLife() const;

	void NotifyServerLifeChanged();

	UPROPERTY(EditAnywhere, BlueprintGetter = GetMaxLife, meta = (ClampMin = 0.0f))
	int32 MaxLife;

	UPROPERTY(BlueprintGetter = GetCurrentLife, ReplicatedUsing = OnRep_CurrentLife)
	int32 CurrentLife;
	UFUNCTION()
	virtual void OnRep_CurrentLife();

	bool bIsAlive;
};
