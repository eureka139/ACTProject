#include "AnimInstance/ACTAnimInstance.h"
#include "Animation/AnimInstance.h"
#include "Character/ACTCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Component/BasicActionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimEnums.h"
#include "Library/ExtensionMathLibrary.h"
#include "Interface/AnimChangeStateInterface.h"
#include "Interface/AttackInterface.h"
#include "ACT/ACTData/ACTEnum.h"
#include "Component/ParryActorComponent.h"

void UACTAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	//获取人物变量引用
	ACTCharacter = Cast<AACTCharacter>(TryGetPawnOwner());
}

void UACTAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	//判断角色引用是否有效
	if (!IsValid(ACTCharacter))
	{
		ACTCharacter = Cast<AACTCharacter>(TryGetPawnOwner());
	}
	if (!IsValid(ACTCharacter))
	{
		return;
	}
	//将角色中的变量与动画实例中实时同步
	MoveAxis = ACTCharacter->MoveAxis;
	CalculateDirection();
	MoveState = ACTCharacter->BasicActionComponent->MoveState;
	bIsCrouch = ACTCharacter->GetCharacterMovement()->IsCrouching();
	bIsFalling = ACTCharacter->GetCharacterMovement()->IsFalling();
	ActiveChildIndex = ACTCharacter->ActiveChildIndex;
	bIsParry = ACTCharacter->ParryActorComponent->bIsParry;
	bIsBlockR = ACTCharacter->ParryActorComponent->bIsBlockR;
	//SetRootMotionMode(bIsFalling == true ? ERootMotionMode::IgnoreRootMotion : ERootMotionMode::RootMotionFromEverything);
	SetRootMotionMode(UExtensionMathLibrary::Select(bIsFalling, ERootMotionMode::RootMotionFromEverything, true, ERootMotionMode::IgnoreRootMotion));
}

void UACTAnimInstance::CalculateDirection()
{
	//判断角色引用是否有效
	if (!IsValid(ACTCharacter))
	{
		ACTCharacter = Cast<AACTCharacter>(TryGetPawnOwner());
	}
	if (!IsValid(ACTCharacter))
	{
		return;
	}
	//该函数用于计算方向，首先获取Actor的控制器旋转值，只取其Yaw值，并以此值获取向前向右向量，分别乘以MoveAxis的Y和X，并将其相加传入CalculateDirection函数中，基础旋转传入Actor当前位于世界坐标的旋转值即可,然后将该值传入Direction变量
	Direction = UAnimInstance::CalculateDirection
	(
		FVector(UKismetMathLibrary::GetForwardVector(FRotator(0, ACTCharacter->GetControlRotation().Yaw, 0)) * MoveAxis.Y) + FVector(UKismetMathLibrary::GetRightVector(FRotator(0, ACTCharacter->GetControlRotation().Yaw, 0)) * MoveAxis.X),
		ACTCharacter->GetActorRotation()
	);
}

void UACTAnimInstance::IChangeState(EPlayerState ACTPlayerState)
{
	//判断角色引用是否有效
	if (!IsValid(ACTCharacter))
	{
		ACTCharacter = Cast<AACTCharacter>(TryGetPawnOwner());
	}
	if (!IsValid(ACTCharacter))
	{
		return;
	}
	IAnimChangeStateInterface* ChangeStateInterface = Cast<IAnimChangeStateInterface>(ACTCharacter);
	if (ChangeStateInterface == nullptr)
	{
		ChangeStateInterface = Cast<IAnimChangeStateInterface>(ACTCharacter);
	}
	if (ChangeStateInterface == nullptr)
	{
		return;
	}
	ChangeStateInterface->ChangeState(ACTPlayerState);
}

void UACTAnimInstance::ICanCombo()
{
	if (!IsValid(ACTCharacter))
	{
		ACTCharacter = Cast<AACTCharacter>(TryGetPawnOwner());
	}
	if (!IsValid(ACTCharacter))
	{
		return;
	}
	IAttackInterface* AttackInterface = Cast<IAttackInterface>(ACTCharacter);
	if (AttackInterface == nullptr)
	{
		AttackInterface = Cast<IAttackInterface>(ACTCharacter);
	}
	if (AttackInterface == nullptr)
	{
		return;
	}
	AttackInterface->CanCombo();
}

void UACTAnimInstance::IResetCombo()
{
	if (!IsValid(ACTCharacter))
	{
		ACTCharacter = Cast<AACTCharacter>(TryGetPawnOwner());
	}
	if (!IsValid(ACTCharacter))
	{
		return;
	}
	IAttackInterface* AttackInterface = Cast<IAttackInterface>(ACTCharacter);
	if (AttackInterface == nullptr)
	{
		AttackInterface = Cast<IAttackInterface>(ACTCharacter);
	}
	if (AttackInterface == nullptr)
	{
		return;
	}
	AttackInterface->ResetCombo();
}

void UACTAnimInstance::AnimNotify_BeginIdle()
{
	if (IsValid(ACTCharacter))
	{
		IChangeState(EPlayerState::EPS_Idle);
	}
}

void UACTAnimInstance::AnimNotify_BeginMove()
{
	if (IsValid(ACTCharacter))
	{
		IChangeState(EPlayerState::EPS_Move);
	}
}

void UACTAnimInstance::AnimNotify_CrouchEnd()
{
	if (IsValid(ACTCharacter))
	{
		IChangeState(EPlayerState::EPS_Idle);
	}
}

void UACTAnimInstance::AnimNotify_JumpEnd()
{
	if (IsValid(ACTCharacter))
	{
		IChangeState(EPlayerState::EPS_Idle);
	}
}

void UACTAnimInstance::AnimNotify_CrouchToMove()
{
	if (IsValid(ACTCharacter))
	{
		IChangeState(EPlayerState::EPS_Move);
	}
}

void UACTAnimInstance::AnimNotify_BeginCrouch()
{
	if (IsValid(ACTCharacter))
	{
		IChangeState(EPlayerState::EPS_Crouch);
	}
}

void UACTAnimInstance::AnimNotify_Equip()
{
	if (IsValid(ACTCharacter))
	{
		IChangeState(EPlayerState::EPS_Equip);
	}
}

void UACTAnimInstance::AnimNotify_CanCombo()
{
	if (IsValid(ACTCharacter))
	{
		ICanCombo();
	}
}

void UACTAnimInstance::AnimNotify_ResetCombo()
{
	if (IsValid(ACTCharacter))
	{
		IResetCombo();
	}
}

void UACTAnimInstance::AnimNotify_BeginBlock()
{
	if (IsValid(ACTCharacter))
	{
		IChangeState(EPlayerState::EPS_ParryStart);
	}
}

void UACTAnimInstance::AnimNotify_Blocking()
{
	if (IsValid(ACTCharacter))
	{
		IChangeState(EPlayerState::EPS_Parrying);
	}
}

void UACTAnimInstance::AnimNotify_BlockEnd()
{
	if (IsValid(ACTCharacter))
	{
		IChangeState(EPlayerState::EPS_Idle);
	}
}
