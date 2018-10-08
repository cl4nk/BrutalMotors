#include "LifeComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "UnrealNetwork.h"

ULifeComponent::ULifeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULifeComponent::AddLifePercent(float const LifePercentToAdd, bool bEvenIfDead)
{
	AddLife(FMath::RoundHalfToEven(LifePercentToAdd * MaxLife), bEvenIfDead);
}

void ULifeComponent::AddLife(int32 const LifeToAdd, bool bEvenIfDead)
{
	if (IsAllowedToModifyLife())
	{
		if (ensure(LifeToAdd > 0))
		{
			bool const bWasAlive = IsAlive();
			if (bWasAlive || bEvenIfDead)
			{
				CurrentLife = FMath::Min(MaxLife, CurrentLife + LifeToAdd);
				NotifyServerLifeChanged();
			}
		}
	}
}

int32 ULifeComponent::GetCurrentLife() const
{
	return CurrentLife;
}

int32 ULifeComponent::GetMaxLife() const
{
	return MaxLife;
}

float ULifeComponent::GetCurrentLifePercent() const
{
	return static_cast<float>(CurrentLife) / static_cast<float>(MaxLife);
}

void ULifeComponent::Reset()
{
	CurrentLife = MaxLife;
	bIsAlive = true;
}

bool ULifeComponent::IsAlive() const
{
	check(CurrentLife >= 0);
	return bIsAlive;
}

// Called when the game starts
void ULifeComponent::BeginPlay()
{
	Super::BeginPlay();

	Reset();

	if (IsAllowedToModifyLife())
	{
		GetOwner()->OnTakeAnyDamage.AddDynamic(this, &ULifeComponent::OnTakeDamage);
	}
}

void ULifeComponent::EndPlay(EEndPlayReason::Type const EndPlayReason)
{
	GetOwner()->OnTakeAnyDamage.RemoveAll(this);

	Super::EndPlay(EndPlayReason);
}

void ULifeComponent::OnRep_CurrentLife()
{
	LifeChanged.Broadcast(GetCurrentLife(), GetCurrentLifePercent());

	bool const bWasAlive = bIsAlive;
	bIsAlive = CurrentLife > 0;
	if (bWasAlive != bIsAlive)
	{
		DeathChanged.Broadcast(bIsAlive);
	}
}

void ULifeComponent::OnTakeDamage(AActor* DamagedActor, float Damage, UDamageType const* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	check(IsAllowedToModifyLife());

	if (IsAlive())
	{
		CurrentLife = FMath::Max(0.0f, CurrentLife - FMath::RoundHalfToEven(Damage));
		NotifyServerLifeChanged();
	}
}

bool ULifeComponent::IsAllowedToModifyLife() const
{
	AActor const* const owner = GetOwner();
	bool const bIsNetRoleCorrect = owner ? owner->Role == ROLE_Authority : false;
	UWorld const* const world = owner ? owner->GetWorld() : nullptr;
	AGameModeBase const* const gameMode = world ? world->GetAuthGameMode() : nullptr;
	return gameMode && bIsNetRoleCorrect;
}

void ULifeComponent::NotifyServerLifeChanged()
{
	if (!IsAllowedToModifyLife())
	{
		OnRep_CurrentLife();
	}
}

void ULifeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ULifeComponent, CurrentLife);
}
