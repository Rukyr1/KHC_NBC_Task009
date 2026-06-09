// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TASKGameStateBase.h"

#include "Kismet/GameplayStatics.h"
#include "Player/TASKPlayerController.h"

void ATASKGameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	// HasAuthority() == false는 '여기가 서버가 아닌 클라이언트 환경인가?'를 묻는 조건문
	// 즉, 이 조건문 때문에 '서버(호스트 플레이어)' 화면에서는 로그인 메시지가 출력되지 않는다
	if (HasAuthority() == false)
	{
		// [주의] GetPlayerController(..., 0)은 '이 컴퓨터(로컬)의 0번 플레이어 컨트롤러'를 가져온다
		// 클라이언트 컴퓨터에는 자기 자신의 플레이어 컨트롤러 딱 1개(0번)만 존재하므로, 
		// 이 코드는 정상적으로 '자기 자신의 컨트롤러'를 찾아온다
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC) == true)
		{
			// 가져온 기본 컨트롤러를 우리가 만든 커스텀 컨트롤러(ATASKPlayerController)로 형변환(Cast)
			ATASKPlayerController* CXPC = Cast<ATASKPlayerController>(PC);
			if (IsValid(CXPC) == true)
			{
				// 로그인한 유저 이름 뒤에 문장을 붙여 알림 텍스트를 생성
				FString NotificationString = InNameString + TEXT(" has joined the game.");
				// 내 화면의 채팅창(혹은 로그)에 이 알림 메시지를 출력
				CXPC->PrintChatMessageString(NotificationString);
			}
		}
	}
}
