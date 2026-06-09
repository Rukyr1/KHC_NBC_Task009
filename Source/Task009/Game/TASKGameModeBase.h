// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TASKGameModeBase.generated.h"

class ATASKPlayerController;

/**
 * 
 */
UCLASS()
class TASK009_API ATASKGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	 // 새로운 플레이어가 서버에 성공적으로 접속(Login)하고, 해당 플레이어를 위한 
	 // 플레이어 컨트롤러(PlayerController) 생성이 완전히 완료되었을 때 호출되는 이벤트 함수
	 // 오직 '서버 환경에서만' 자동으로 실행되므로, 새로운 유저가 들어왔을 때 
	 // 닉네임을 환영 메시지로 띄우거나 초기 아이템을 지급하는 등의 서버측 처리를 하기에 가장 안전한 타이밍
	virtual void OnPostLogin(AController* NewPlayer) override;
	
	// 숫자 야구 ↓
	FString GenerateSecretNumber();

	bool IsGuessNumberString(const FString& InNumberString);

	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);
	
	// GameMode에 로직 적용 ↓
	virtual void BeginPlay() override;
	
	void PrintChatMessageString(ATASKPlayerController* InChattingPlayerController, const FString& InChatMessageString);

protected:
	FString SecretNumberString;

	TArray<TObjectPtr<ATASKPlayerController>> AllPlayerControllers;
};
