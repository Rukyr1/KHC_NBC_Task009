// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TASKPawn.generated.h"

UCLASS()
class TASK009_API ATASKPawn : public APawn
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// 플레이어나 AI(Controller)가 이 캐릭터를 '조종하기 시작하는 순간' 실행되는 함수
	// ⚠️ 중요: 멀티플레이 게임에서 오직 '서버(Server)'에서만 호출되는 특성이 있어서, 네트워크 권한을 설정할 때 핵심
	virtual void PossessedBy(AController* NewController) override;

};
