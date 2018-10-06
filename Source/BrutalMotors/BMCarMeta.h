// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BMCarMeta.generated.h"

UENUM(BlueprintType)
enum class EBMArchetype : uint8
{
	None,
	Meca,
	Techno,
	Magic
};

USTRUCT(BlueprintType)
struct FBMArchetypeItem
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<EBMArchetype> Archetype;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = 0))
	int32 Id;

	bool operator==(const FBMArchetypeItem& rhs) const
	{
		return Archetype == rhs.Archetype && Id == rhs.Id;
	}
};

inline uint32 GetTypeHash(const FBMArchetypeItem & Item)
{
	return FCrc::MemCrc_DEPRECATED(&Item, sizeof(Item));
}