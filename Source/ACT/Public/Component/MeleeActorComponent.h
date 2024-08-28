#pragma once

#include "CoreMinimal.h"
#include "ACT/ACTData/ACTEnum.h"
#include "Interface/AttackInterface.h"
#include "Components/ActorComponent.h"
#include "MeleeActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACT_API UMeleeActorComponent : public UActorComponent ,public IAttackInterface
{
	GENERATED_BODY()

public:	

	UMeleeActorComponent();

	friend class AACTCharacter;
	//初始化
	virtual void BeginPlay() override;
	//实时更新
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//普通攻击
	void Attack();
	//蓄力攻击
	void ChargeAttack();
	UFUNCTION()
	void ChargeSuccess(FName NotifyName);

	UFUNCTION()
	void EnableTrace(FName NotifyName);

	UFUNCTION()
	void DisableTrace(FName NotifyName);
	UFUNCTION()
	void ResetCombo(FName NotifyName);

protected:



private:

	//变量引用与角色中定义
	class AACTCharacter* ACTCharacter;

	//攻击次数
	int32 AttackCount = -1;

public:	

	UPROPERTY()
	class UExtensionMontageProxy* BeginAttackPlayMontageProxy;
	UPROPERTY()
	UExtensionMontageProxy* LastAttackPlayMontageProxy;
	UPROPERTY()
	UExtensionMontageProxy* ChargeAttackPlayMontageProxy;
	UPROPERTY()
	UExtensionMontageProxy* ChargeSuccessPlayMontageProxy;
		
};
