#pragma once

#include "CoreMinimal.h"
#include "ACT/ACTData/ACTEnum.h"
#include "Components/ActorComponent.h"
#include "BasicActionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACT_API UBasicActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UBasicActionComponent();

	friend class AACTCharacter;
	//初始化
	virtual void BeginPlay() override;
	//实时更新
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//更改移动状态
	void MoveChangeState();
	//更改蹲伏状态
	void Crouching();
	//更改跳跃状态
	void JumpStart();
	void JumpEnd();
	//更改持刀状态
	void Equiped();
	//调用委托时更改状态函数
	UFUNCTION()
	void EquipeChangeState(FName NotifyName);
	//调用委托时更改装备武器变量
	UFUNCTION()
	void EquipeChangeActiveIndex(FName NotifyName);

protected:

private:
	//变量引用与角色中定义
	class AACTCharacter* ACTCharacter;

public:	
	//移动状态
	float MoveState;

	UPROPERTY()
	class UExtensionMontageProxy* EquipPlayMontageProxy;
	UPROPERTY()
	class UExtensionMontageProxy* UnarmPlayMontageProxy;

};
