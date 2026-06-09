// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TASKPawn.h"

#include "Task009.h"


void ATASKPawn::BeginPlay()
{
	Super::BeginPlay();

	// 이 캐릭터의 현재 로컬/원격 역할(예: ROLE_Authority 등)을 문자열로 가져온다
	FString NetRoleString = ChatXFunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("CXPawn::BeginPlay() %s [%s]"), *ChatXFunctionLibrary::GetNetModeString(this), *NetRoleString);
	ChatXFunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}

void ATASKPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 조종이 시작된 순간의 캐릭터 네트워크 역할을 문자열로 가져온다
	FString NetRoleString = ChatXFunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("CXPawn::PossessedBy() %s [%s]"), *ChatXFunctionLibrary::GetNetModeString(this), *NetRoleString);
	ChatXFunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}
