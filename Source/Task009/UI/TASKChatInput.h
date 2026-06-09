// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TASKChatInput.generated.h"

class UEditableTextBox;

/**
 * 
 */
UCLASS()
class TASK009_API UTASKChatInput : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 위젯이 화면에 나타나기 직전에 딱 한 번 호출
	virtual void NativeConstruct() override;

	// 위젯이 화면에서 사라지거나 메모리에서 소멸될 때 호출
	virtual void NativeDestruct() override;

protected:
	UFUNCTION()
	// 채팅 입력 확인
	void OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	
public:
	// meta = (): ()변수 이름과 똑같은 이름의 위젯을 블루프린트 UI(UMG) 디자인 창에 반드시 구현 강제
	UPROPERTY(meta =(BindWidget))
	// TObjectPtr<...>: 언리얼 엔진 5에서 새로 도입된 최신 객체 포인터 표준 스타일
	// 기존 UE4에서 쓰던 일반 포인터(UEditableTextBox*)를 대체
	TObjectPtr<UEditableTextBox> TASKChatInput;
};
