#pragma once

#include "InputActionValue.h"
#include "CoreMinimal.h"
#include "Interface/AnimChangeStateInterface.h"
#include "Interface/AttackInterface.h"
#include "ACT/ACTData/ACTEnum.h"
#include "GameFramework/Character.h"
#include "ACTCharacter.generated.h"

UCLASS()
class ACT_API AACTCharacter : public ACharacter , public IAnimChangeStateInterface, public IAttackInterface
{
	GENERATED_BODY()

public:

	AACTCharacter();

	//重载接口函数
	virtual void ChangeState(EPlayerState ACTPlayerState) override;
	virtual void CanCombo() override;
	virtual void ResetCombo() override;

	//初始化
	virtual void BeginPlay() override;
	//实时更新
	virtual void Tick(float DeltaTime) override;
	//设置角色输入组件
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//当所有子组件初始化完毕时调用
	virtual void PostInitializeComponents() override;
	//移动操作
	void Move(const FInputActionValue& InputValue);
	//行走奔跑状态切换
	void WalkRun(const FInputActionValue& InputValue);
	//蹲伏行走奔跑状态切换
	void Crouching(const FInputActionValue& InputValue);
	//跳跃状态切换
	void JumpStart(const FInputActionValue& InputValue);
	void JumpEnd(const FInputActionValue& InputValue);
	//视角移动
	void Lock(const FInputActionValue& InputValue);
	//装备武器
	void Equiped(const FInputActionValue& InputValue);
	//普通攻击
	void Attack(const FInputActionValue& InputValue);
	//蓄力攻击
	void ChargeAttack(const FInputActionValue& InputValue);
	//闪避
	void Avoid(const FInputActionValue& InputValue);
	//锁定
	void Locked(const FInputActionValue& InputValue);
	//格挡
	void ParryStart(const FInputActionValue& InputValue);

	void ParryEnd(const FInputActionValue& InputValue);

	void EnableTrace();

	void DisableTrace();

	void LeftLocked();

	void RightLocked();



	

protected:

private:
	//弹簧臂组件
	UPROPERTY(VisibleAnywhere, Category = "Camera", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	//增强输入本地玩家子系统
	UPROPERTY()
	class UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem;
	//增强输入组件
	UPROPERTY()
	class UEnhancedInputComponent* EnhancedInputComponent;
	//输入映射上下文
	UPROPERTY(EditAnywhere, Category = "EnhancedInput", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> InputMappingContext;
	//移动操作
	UPROPERTY(EditAnywhere, Category = "EnhancedInput", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;
	//跳跃操作
	UPROPERTY(EditAnywhere, Category = "EnhancedInput", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;
	//蹲伏操作
	UPROPERTY(EditAnywhere, Category = "EnhancedInput", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> CrouchAction;
	//移动状态切换操作
	UPROPERTY(EditAnywhere, Category = "EnhancedInput", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> WalkRunAction;
	//视角移动操作
	UPROPERTY(EditAnywhere, Category = "EnhancedInput", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LockAction;
	//装备武器操作
	UPROPERTY(EditAnywhere, Category = "EnhancedInput", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> EquipAction;
	//攻击操作
	UPROPERTY(EditAnywhere, Category = "EnhancedInput", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;
	//蓄力攻击操作
	UPROPERTY(EditAnywhere, Category = "EnhancedInput", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ChargeAttackAction;
	//闪避操作
	UPROPERTY(EditAnywhere, Category = "EnhancedInput", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AvoidAction;
	//锁定操作
	UPROPERTY(EditAnywhere, Category = "EnhancedInput", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LockedAction;
	//锁定上移操作
	UPROPERTY(EditAnywhere, Category = "EnhancedInput", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LockedLeftAction;
	//锁定下移操作
	UPROPERTY(EditAnywhere, Category = "EnhancedInput", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LockedRightAction;
	//格挡操作
	UPROPERTY(EditAnywhere, Category = "EnhancedInput", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ParryAction;

public:	

	//玩家控制器
	UPROPERTY()
	class APlayerController* PlayerController;

	//摄像机组件
	UPROPERTY(VisibleAnywhere, Category = "Camera", BlueprintReadWrite)
	class UCameraComponent* Camera;

	//移动输入方向轴值
	UPROPERTY(EditAnywhere, Category = "Movement", BlueprintReadWrite)
	FVector2D MoveAxis;

	//基础移动状态组件
	UPROPERTY()
	class UBasicActionComponent* BasicActionComponent;

	//近战系统组件
	UPROPERTY()
	class UMeleeActorComponent* MeleeActionComponent;
	//伤害系统组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDamageActorComponent* DamageActorComponent;
	//闪避系统组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USprintActorComponent* SprintActorComponent;

	//锁定系统组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ULockedActorComponent* LockedActorComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParryActorComponent* ParryActorComponent;

	//刀网格体
	UPROPERTY(EditAnywhere, Category = "Katana", BlueprintReadWrite)
	class UStaticMeshComponent* KatanaMesh;

	//刀鞘网格体
	UPROPERTY(EditAnywhere, Category = "Katana", BlueprintReadWrite)
	class UStaticMeshComponent* ScabbardMesh;

	//射线检测插槽
	UPROPERTY(EditAnywhere, Category = "Katana", BlueprintReadWrite)
	class USceneComponent* TraceStart;
	UPROPERTY(EditAnywhere, Category = "Katana", BlueprintReadWrite)
	USceneComponent* Trace1;
	UPROPERTY(EditAnywhere, Category = "Katana", BlueprintReadWrite)
	USceneComponent* Trace2;
	UPROPERTY(EditAnywhere, Category = "Katana", BlueprintReadWrite)
	USceneComponent* Trace3;
	UPROPERTY(EditAnywhere, Category = "Katana", BlueprintReadWrite)
	USceneComponent* TraceEnd;

	//角色基础状态枚举
	UPROPERTY(EditAnywhere, Category = "Enum", BlueprintReadWrite)
	EPlayerState ACTPlayerState;

	//装备切换整数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	int32 ActiveChildIndex;

	//装备切换蒙太奇
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipAnimMontage")
	class UAnimMontage* EquipMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EquipAnimMontage")
	UAnimMontage* UnarmMontage;
	//攻击蒙太奇数组
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackAnimMontage")
	TArray<UAnimMontage*> AttackMontage;
	//蓄力攻击蒙太奇
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChargeAttackMontage")
	class UAnimMontage* ChargeAttackMontage;

	//受击蒙太奇
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitMontage")
	class UAnimMontage* HitMontage;

	//闪避蒙太奇
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitMontage")
	class UAnimMontage* AvoidMontage;

	//格挡蒙太奇
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitMontage")
	class UAnimMontage* ParryMontage;
};
