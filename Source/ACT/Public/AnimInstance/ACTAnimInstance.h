#pragma once

#include "CoreMinimal.h"
#include "Interface/AnimChangeStateInterface.h"
#include "Interface/AttackInterface.h"
#include "Animation/AnimInstance.h"
#include "ACTAnimInstance.generated.h"

UCLASS()
class ACT_API UACTAnimInstance : public UAnimInstance , public IAnimChangeStateInterface , public IAttackInterface
{
	GENERATED_BODY()
public:
	//事件开始初始化
	virtual void NativeInitializeAnimation() override;

	//实时更新动画
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	//计算控制器旋转方向
	void CalculateDirection();

	//通过接口更改角色枚举状态
	void IChangeState(EPlayerState ACTPlayerState);

	//通过接口来更改角色攻击枚举状态
	void ICanCombo();
	void IResetCombo();



	//通过动画通知更改角色枚举状态
	UFUNCTION()
	void AnimNotify_BeginIdle();
	UFUNCTION()
	void AnimNotify_BeginMove();
	UFUNCTION()
	void AnimNotify_CrouchEnd();
	UFUNCTION()
	void AnimNotify_JumpEnd();
	UFUNCTION()
	void AnimNotify_CrouchToMove();
	UFUNCTION()
	void AnimNotify_BeginCrouch();
	UFUNCTION()
	void AnimNotify_Equip();
	UFUNCTION()
	void AnimNotify_CanCombo();
	UFUNCTION()
	void AnimNotify_ResetCombo();
	UFUNCTION()
	void AnimNotify_BeginBlock();
	UFUNCTION()
	void AnimNotify_Blocking();
	UFUNCTION()
	void AnimNotify_BlockEnd();

protected:


private:

	//通过类型转换获取角色的引用，从而来调用角色中的变量信息
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	class AACTCharacter* ACTCharacter;

	//角色移动方向（-180 --- +180）
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	float Direction;
	//角色移动状态（走，跑，蹲走，蹲跑）
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	float MoveState;
	//人物移动状态键值（自角色中引入）
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	FVector2D MoveAxis;
	//是否处于蹲伏状态
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	bool bIsCrouch;
	//是否处于跳跃状态
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	bool bIsFalling;
	//角色未持刀与持刀状态
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	int32 ActiveChildIndex;
	//角色格挡状态
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	bool bIsParry;
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	bool bIsBlockR;

};
