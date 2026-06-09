// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TASKChatInput.h"

#include "Components/EditableTextBox.h"
#include "Player/TASKPlayerController.h"

// 위젯이 게임 화면에 나타날 때 딱 한 번 실행되는 함수
void UTASKChatInput::NativeConstruct()
{
	Super::NativeConstruct();
	
	// IsAlreadyBound로 이미 연결되어 있는지 먼저 검사하여, 중복 연결로 인해 이벤트가 두 번 실행되는 버그를 방지
	if (TASKChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == false)
	{
		// AddDynamic을 통해 텍스트 입력창(TASKChatInput)에 입력 완료 신호(OnTextCommitted)가 오면, 밑에 있는 OnChatInputTextCommitted 함수를 실행하라고 자석처럼 연결
		TASKChatInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextCommitted);		
	}	
}

// 위젯이 화면에서 사라지거나 완전히 파괴될 때 실행되는 함수
void UTASKChatInput::NativeDestruct()
{
	Super::NativeDestruct();
	
	// RemoveDynamic을 통해 연결해 두었던 이벤트를 다시 끊어준다
	// 이 과정이 없으면 위젯이 사라진 후에도 찌꺼기가 남아 게임이 튕기는 메모리 누수 및 크래시 현상(댕글링 포인터)을 예방
	if (TASKChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == true)
	{
		TASKChatInput->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UTASKChatInput::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	// 엔터키(OnEnter)를 쳤을 때만 작동
	if (CommitMethod == ETextCommit::OnEnter)
	{
		// 이 UI를 조종하고 있는(소유한) 플레이어 컨트롤러를 가져온다
		APlayerController* OwningPlayerController = GetOwningPlayer();
		if (IsValid(OwningPlayerController) == true)
		{
			// 대화 내용을 저장할 기능이 있는 진짜 알맹이 클래스(ATASKPlayerController)로 안전하게 형변환(Cast)
			ATASKPlayerController* OwningTASKPlayerController = Cast<ATASKPlayerController>(OwningPlayerController);
			if (IsValid(OwningTASKPlayerController) == true)
			{
				// 다국어 전용 텍스트(FText)를 문자열(FString)로 가볍게 변환하여 컨트롤러에 배달
				OwningTASKPlayerController->SetChatMessageString(Text.ToString());
				
				// 배달이 끝났으므로 화면의 입력창 글자를 깨끗하게 비워준다(SetText)
				TASKChatInput->SetText(FText());
			}
		}
	}
}
