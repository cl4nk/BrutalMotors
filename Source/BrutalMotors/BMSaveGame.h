// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "BMCarMeta.h"
#include "BMSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class BRUTALMOTORS_API UBMSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite)
	TArray<FBMCarItem> CarItems;
};
