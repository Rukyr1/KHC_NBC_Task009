// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TASKGameModeBase.h"

#include "EngineUtils.h"
#include "TASKGameStateBase.h"
#include "Player/TASKPlayerController.h"
#include "Player/TASKPlayerState.h"

// 새로운 플레이어가 게임 세션에 정상적으로 참여 완료했을 때 서버에서 실행되는 함수
void ATASKGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	
	// // 현재 게임 월드에 상주하며 클라이언트들과 데이터를 공유하는 GameState를 가져온다
	// // 우리가 만든 커스텀 클래스인 ATASKGameStateBase 형태로 안전하게 형변환
	// ATASKGameStateBase* TaskGameStateBase = GetGameState<ATASKGameStateBase>();
	// // GameState가 메모리에 잘 존재하고 유효한지 검사
	// if (IsValid(TaskGameStateBase) == true)
	// {
	// 	// [핵심 네트워크 통신] 
	// 	// GameState에 구현된 멀티캐스트 RPC 함수를 호출
	// 	// 이 명령 하나로 서버를 포함해 현재 게임에 접속해 있는 '모든 클라이언트 컴퓨터'에서 
	// 	// "XXXXXXX"라는 텍스트를 인자로 가진 로그인 브로드캐스트 함수가 동시에 실행
	// 	TaskGameStateBase->MulticastRPCBroadcastLoginMessage(TEXT("XXXXXXX"));
	// }
	// // GameMode에 로직 적용 ↓
	// ATASKPlayerController* CXPlayerController = Cast<ATASKPlayerController>(NewPlayer);
	// if (IsValid(CXPlayerController) == true)
	// {
	// 	AllPlayerControllers.Add(CXPlayerController);
	// }
	
	ATASKPlayerController* CXPlayerController = Cast<ATASKPlayerController>(NewPlayer);
	if (IsValid(CXPlayerController) == true)
	{
		CXPlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server."));
		
		AllPlayerControllers.Add(CXPlayerController);

		ATASKPlayerState* CXPS = CXPlayerController->GetPlayerState<ATASKPlayerState>();
		if (IsValid(CXPS) == true)
		{
			CXPS->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		}

		ATASKGameStateBase* CXGameStateBase =  GetGameState<ATASKGameStateBase>();
		if (IsValid(CXGameStateBase) == true)
		{
			CXGameStateBase->MulticastRPCBroadcastLoginMessage(CXPS->PlayerNameString);
		}
	}
}

// 숫자 야구 ↓
FString ATASKGameModeBase::GenerateSecretNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}

	FMath::RandInit(FDateTime::Now().GetTicks());
	Numbers = Numbers.FilterByPredicate([](int32 Num) { return Num > 0; });
	
	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}

	return Result;
}

bool ATASKGameModeBase::IsGuessNumberString(const FString& InNumberString)
{
	bool bCanPlay = false;

	do {

		if (InNumberString.Len() != 3)
		{
			break;
		}

		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				break;
			}
			
			UniqueDigits.Add(C);
		}

		if (bIsUnique == false)
		{
			break;
		}

		bCanPlay = true;
		
	} while (false);	

	return bCanPlay;
}

FString ATASKGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else 
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;				
			}
		}
	}

	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}

// GameMode에 로직 적용 ↓
void ATASKGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	SecretNumberString = GenerateSecretNumber();
	UE_LOG(LogTemp, Error, TEXT("%s"), *SecretNumberString);
}

void ATASKGameModeBase::PrintChatMessageString(ATASKPlayerController* InChattingPlayerController,
	const FString& InChatMessageString)
{
	if (IsValid(InChattingPlayerController) == false) { return; }
	int Index = InChatMessageString.Len() - 3;
	FString GuessNumberString = InChatMessageString.RightChop(Index);
	
	if (IsGuessNumberString(GuessNumberString) == true)
	{
		FString JudgeResultString = JudgeResult(SecretNumberString, GuessNumberString);

		IncreaseGuessCount(InChattingPlayerController);
		
		for (TActorIterator<ATASKPlayerController> It(GetWorld()); It; ++It)
		{
			ATASKPlayerController* CXPlayerController = *It;
			if (IsValid(CXPlayerController) == true)
			{
				FString CombinedMessageString = InChatMessageString + TEXT(" -> ") + JudgeResultString;
				CXPlayerController->ClientRPCPrintChatMessageString(CombinedMessageString);

				int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
				JudgeGame(InChattingPlayerController, StrikeCount);
			}
		}
	}
	else
	{
		for (TActorIterator<ATASKPlayerController> It(GetWorld()); It; ++It)
		{
			ATASKPlayerController* CXPlayerController = *It;
			if (IsValid(CXPlayerController) == true)
			{
				CXPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
			}
		}
	}
}

void ATASKGameModeBase::IncreaseGuessCount(ATASKPlayerController* InChattingPlayerController)
{
	ATASKPlayerState* CXPS = InChattingPlayerController->GetPlayerState<ATASKPlayerState>();
	if (IsValid(CXPS) == true)
	{
		CXPS->CurrentGuessCount++;
	}
}

void ATASKGameModeBase::ResetGame()
{
	SecretNumberString = GenerateSecretNumber();

	for (const auto& TASKPlayerController : AllPlayerControllers)
	{
		ATASKPlayerState* CXPS = TASKPlayerController->GetPlayerState<ATASKPlayerState>();
		if (IsValid(CXPS) == true)
		{
			CXPS->CurrentGuessCount = 0;
		}
	}
}

void ATASKGameModeBase::JudgeGame(ATASKPlayerController* InChattingPlayerController, int InStrikeCount)
{
	if (3 == InStrikeCount)
	{
		ATASKPlayerState* CXPS = InChattingPlayerController->GetPlayerState<ATASKPlayerState>();
		for (const auto& TASKPlayerController : AllPlayerControllers)
		{
			if (IsValid(CXPS) == true)
			{
				FString CombinedMessageString = CXPS->PlayerNameString + TEXT(" has won the game.");
				TASKPlayerController->NotificationText = FText::FromString(CombinedMessageString);
				
				ResetGame();
			}
		}
	}
	else
	{
		bool bIsDraw = true;
		for (const auto& TASKPlayerController : AllPlayerControllers)
		{
			ATASKPlayerState* CXPS = TASKPlayerController->GetPlayerState<ATASKPlayerState>();
			if (IsValid(CXPS) == true)
			{
				if (CXPS->CurrentGuessCount < CXPS->MaxGuessCount)
				{
					bIsDraw = false;
					break;
				}
			}
		}

		if (true == bIsDraw)
		{
			for (const auto& TASKPlayerController : AllPlayerControllers)
			{
				TASKPlayerController->NotificationText = FText::FromString(TEXT("Draw..."));
			}
			
			ResetGame();
		}
	}
}
