// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "NS_TraceSpawn.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UNS_TraceSpawn : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Connect Niagara")
	int32 ConnectIndex = 0;

	// �߰� ������ �����Դϴ�. 0�̸� �⺻ ���ݷ¸�ŭ �������� ������, -5�� �⺻ ���ݷ¿��� 5�� ����ŭ �������� �����ϴ�.
	UPROPERTY(EditAnywhere, Category = "Connect Niagara")
	float AddDamage = 0;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
