// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TASKPlayerController.generated.h"

class UTASKChatInput;
class UUserWidget;

/**
 * 
 */
UCLASS()
class TASK009_API ATASKPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ATASKPlayerController();
	
	virtual void BeginPlay() override;
	
	// 채팅 입력
	void SetChatMessageString(const FString& InChatMessageString);
	
	// 채팅 출력
	void PrintChatMessageString(const FString& InChatMessageString);
	
	// Client RPC (Remote Procedure Call)
	// 서버가 호출하지만, 실제 실행은 '해당 컨트롤러를 소유한 클라이언트'에서만 실행되는 함수
	// 주로 서버의 데이터를 특정 개인 클라이언트 UI에 반영할 때 사용
	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessageString(const FString& InChatMessageString);
	
	// Server RPC (Remote Procedure Call)
	// 클라이언트가 호출하여 서버 환경에서 실행되도록 요청하는 함수
	// 클라이언트가 친 채팅을 서버로 전송하여 모든 사람에게 전파하기 전 검증 및 전달 처리
	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessageString(const FString& InChatMessageString);
	
	// 엔터키를 누를 때마다 채팅창을 켜고 끌 함수
	void OnToggleChat();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UTASKChatInput> ChatInputWidgetClass; // TSubClassOf: 붕어빵 틀, 에디터에서 넣기
	
	UPROPERTY()
	TObjectPtr<UTASKChatInput> ChatInputWidgetInstance; // TObjectPtr: 붕어빵, C++에서 만듬
	
	// 입력 받은 채팅 문자열
	FString ChatMessageString;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> NotificationTextWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> NotificationTextWidgetInstance;
	
private:
	// 채팅 가능 여부
	bool bIsChatActive;
	
public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	FText NotificationText;
};
