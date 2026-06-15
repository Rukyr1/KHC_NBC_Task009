// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TASKPlayerController.h"

#include "EngineUtils.h"
#include "Task009.h"
#include "TASKPlayerState.h"
#include "Game/TASKGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "UI/TASKChatInput.h"

ATASKPlayerController::ATASKPlayerController()
{
	bReplicates = true;
}

void ATASKPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocalController() == false) { return; }
	
	// 입력을 오직 UI에만 집중시키겠다는 모드 변수를 생성
	FInputModeUIOnly InputModeUIOnly;
	// 실제로 플레이어의 키보드/마우스 입력을 UI 조작용으로만 제한하여 적용
	SetInputMode(InputModeUIOnly);

	// 에디터에서 디자이너가 UI 설계도(Class)를 빼먹지 않고 올바르게 등록했는지 안전하게 검사
	if (IsValid(ChatInputWidgetClass) == true)
	{
		// 클래스를 바탕으로 게임 메모리에 '진짜 UI 객체(Instance)'를 굽고(생성), 주소값을 저장합니다.
		// UTASKChatInput 클래스 형태로 UI 객체를 새로 하나 인스턴스화
		// this: 자신 (여기서는 PlayerController)
		// ChatInputWidgetClass: 원본 클래스(그대로 복사해서 서식 넘기기)
		ChatInputWidgetInstance = CreateWidget<UTASKChatInput>(this, ChatInputWidgetClass);
		// 메모리에 진짜 UI 객체가 에러 없이 성공적으로 잘 만들어졌는지 2차로 확인
		if (IsValid(ChatInputWidgetInstance) == true)
		{
			// 메모리에만 존재하던 UI 객체를 플레이어가 보는 실제 모니터 화면(뷰포트) 띄우기
			ChatInputWidgetInstance->AddToViewport();
		}
	}
	
	if (IsValid(NotificationTextWidgetClass) == true)
	{
		NotificationTextWidgetInstance = CreateWidget<UUserWidget>(this, NotificationTextWidgetClass);
		if (IsValid(NotificationTextWidgetInstance) == true)
		{
			NotificationTextWidgetInstance->AddToViewport();
		}
	}
}

// 입력된 문자열 저장
void ATASKPlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;
	
	// PrintChatMessageString(ChatMessageString);
	if (IsLocalController() == true)
	{
		//ServerRPCPrintChatMessageString(InChatMessageString);	
		ATASKPlayerState* CXPS = GetPlayerState<ATASKPlayerState>();
		if (IsValid(CXPS) == true)
		{
			FString CombinedMessageString = CXPS->PlayerNameString + TEXT(": ") + InChatMessageString;

			ServerRPCPrintChatMessageString(CombinedMessageString);
		}
	}
}

// 입력된 문자열 출력(채팅)
void ATASKPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	//UKismetSystemLibrary::PrintString(this, ChatMessageString, true, true, FLinearColor::Red, 5.0f);
	
	// FString NetModeString = ChatXFunctionLibrary::GetNetModeString(this);
	// FString CombinedMessageString = FString::Printf(TEXT("%s: %s"), *NetModeString, *InChatMessageString);
	// ChatXFunctionLibrary::MyPrintString(this, CombinedMessageString, 10.f);
	
	ChatXFunctionLibrary::MyPrintString(this, InChatMessageString, 10.f);
}

void ATASKPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, NotificationText);
}

// 서버가 특정 클라이언트에게 실행을 명령한 RPC의 실제 본문 로직
void ATASKPlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	// 전달받은 문자열을 로컬 출력 함수로 전달
	PrintChatMessageString(InChatMessageString);
}

// 클라이언트가 채팅을 친 뒤 서버에게 전송했을 때, '서버 환경'에서 실행되는 RPC 본문 로직
void ATASKPlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	// // [전체 전파 로직] 현재 게임 월드에 존재하는 모든 플레이어 컨트롤러를 하나씩 순회(루프)
	// for (TActorIterator<ATASKPlayerController> It(GetWorld()); It; ++It)
	// {
	// 	// 반복문에서 현재 가리키고 있는 플레이어 컨트롤러 포인터 추출
	// 	ATASKPlayerController* CXPlayerController = *It;
	// 	// 해당 컨트롤러가 유효한지(메모리가 살아있는지) 안전하게 확인
	// 	if (IsValid(CXPlayerController) == true)
	// 	{
	// 		// 해당 플레이어 컨트롤러를 소유한 클라이언트에게 "이 채팅 화면에 그려라" 하고 Client RPC를 원격 호출
	// 		CXPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
	// 	}
	// }
	
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM) == true)
	{
		ATASKGameModeBase* CXGM = Cast<ATASKGameModeBase>(GM);
		if (IsValid(CXGM) == true)
		{
			CXGM->PrintChatMessageString(this, InChatMessageString);
		}
	}
}
