#pragma once

#include "ACTEnum.generated.h"

//角色基础状态
UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	EPS_Idle UMETA(DisplayName = "Idle"), //待机
	EPS_Move UMETA(DisplayName = "Move"), //移动
	EPS_Jump UMETA(DisplayName = "Jump"), //跳跃
	EPS_Crouch UMETA(DisplayName = "Crouch"), //蹲伏
	EPS_Equip UMETA(DisplayName = "Equip"), //装备武器
	EPS_Attack UMETA(DisplayName = "Attack"), //普通攻击
	EPS_ChargeAttack UMETA(DisplayName = "ChargeAttack"), //是否能够进行蓄力攻击
	EPS_ChargeSuccess UMETA(DisplayName = "ChargeSuccess"), //进行蓄力攻击
	EPS_Avoid UMETA(DisplayName = "Avoid"), //闪避
	EPS_Hit UMETA(DisplayName = "Hit"), //受击
	EPS_ParryStart UMETA(DisplayName = "ParryStart"), //开始格挡
	EPS_Parrying UMETA(DisplayName = "Parrying"), //格挡中
};

UENUM(BlueprintType)
enum class ELockedType : uint8
{
	ELP_LeftLock UMETA(DisplayName = "LeftLock"),
	ELP_RightLock UMETA(DisplayName = "RightLock")
};