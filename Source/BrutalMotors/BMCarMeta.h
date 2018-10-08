// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BMCarMeta.generated.h"

UENUM(BlueprintType)
enum class EBMArchetype : uint8
{
	None = 0,
	Meca,
	Techno,
	Magic
};

USTRUCT(BlueprintType)
struct FBMCarItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<EBMArchetype> Archetype;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0))
	int32 Id;

	bool operator==(FBMCarItem const& rhs) const
	{
		return Archetype == rhs.Archetype && Id == rhs.Id;
	}
};

inline uint32 GetTypeHash(FBMCarItem const& Item)
{
	return FCrc::MemCrc32(&Item, sizeof(Item));
}

USTRUCT(BlueprintType)
struct FBMCarConfig
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBMCarItem Wheel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBMCarItem Frame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBMCarItem PrimaryShoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBMCarItem SecondaryShoot;
};