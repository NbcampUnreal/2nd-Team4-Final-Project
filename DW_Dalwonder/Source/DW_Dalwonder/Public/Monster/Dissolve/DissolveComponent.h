// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DissolveComponent.generated.h"

/**
* 디졸브 이펙트를 작동시키기 위한 컴포넌트.
* 텍스쳐, 나이아가라 시스템 에셋 할당 필수.
*/ 
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DW_DALWONDER_API UDissolveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDissolveComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 디졸브를 실행시킬 함수입니다. 첫 번째 인자로 텍스쳐의 인덱스를, 두 번째 인자로 지속시간을 받습니다. 
	UFUNCTION(BlueprintCallable)
	void DissolveStart(int32 TextureIndex, float Duration);

protected:
	// 디졸브가 시작한 시점에서 Duration만큼이 지난 뒤 실행할 함수.
	void DissolveZero();
	// DissolveZero가 실행되고 1초에 걸쳐 완전히 이펙트를 끝낼 함수.
	void DissolveEnd();

public:
	// 디졸브 텍스쳐입니다. 머터리얼과 나이아가라 시스템 모두 할당됩니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A_Dissolve|Class")
	TArray<UTexture*> DissolveTexture;
	// 할당한 디졸브 텍스쳐를 어디서든 랜덤하게 할당시킬지 여부입니다. True일 경우 호출되는 DissolveStart의 텍스쳐 인덱스는 무시됩니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A_Dissolve|Class")
	bool bApplyRandomTexture;

	// 디졸브 테두리의 색상입니다. 머터리얼에만 할당됩니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A_Dissolve|Material")
	FLinearColor MaterialEdgeColor = FLinearColor::White;
	// 디졸브 테두리에 지정한 색상의 두께입니다. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A_Dissolve|Material")
	float MaterialEdgeThickness = 0.0f;

	// 디졸브 테두리의 파티클 색상입니다. 나이아가라 시스템에 할당됩니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A_Dissolve|Niagara")
	FLinearColor DissolveParticleColor = FLinearColor::White;
	// 흩날리는 파티클 색상입니다. 나이아가라 시스템에 할당됩니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "A_Dissolve|Niagara")
	FLinearColor DustParticleColor = FLinearColor::White;


private:
	FName DissolveAmountParameterName = "Amount";
	FName TextureParameterName = "DissolveTexture";
	FName ColorParameterName = "EdgeColor";
	FName DissolveEdgeParameterName = "EdgeThickness";

	TMap<int32, UMaterialInstanceDynamic*> DynamicMaterialMap;

	USkeletalMeshComponent* MeshComp;

	float DissolveDuration = 5.f;
	float CurrentTime = 0.f;

	float StartValue = 0.f;
	float EndValue = 1.f;
	float EndValue_NS = -1.f;

	FTimerHandle ZeroTimer;
	FTimerHandle EndTimer;
	
	bool bIsDissolveFirst;
};
