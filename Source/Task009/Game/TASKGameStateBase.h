// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TASKGameStateBase.generated.h"

class ATASKPlayerController;

/**
 * 
 */
UCLASS()
class TASK009_API ATASKGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	// Multicast RPC (NetMulticast)
	// 서버가 이 함수를 호출하면, 서버를 포함하여 현재 게임에 접속해 있는 
	// '모든 클라이언트'에서 동시에 실행되는 네트워크 함수
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadcastLoginMessage(const FString& InNameString = FString(TEXT("XXXXXXX")));
	
	FString GenerateSecretNumber();

	bool IsGuessNumberString(const FString& InNumberString);

	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);
};
