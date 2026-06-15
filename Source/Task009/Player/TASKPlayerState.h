// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TASKPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class TASK009_API ATASKPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ATASKPlayerState();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	FString GetPlayerInfoString();
	
	UPROPERTY(Replicated)
	FString PlayerNameString;
	
	
	UPROPERTY(Replicated)
	int32 CurrentGuessCount;

	UPROPERTY(Replicated)
	int32 MaxGuessCount;
	
};
