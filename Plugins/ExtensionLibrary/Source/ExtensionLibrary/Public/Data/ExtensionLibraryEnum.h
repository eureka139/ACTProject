#pragma once

/**
 *
 */

/////////////////////////////////////////////////////////////////////////////////

/**
 *这个枚举值代表插值的速度变化曲线。
 */
UENUM()
enum class EExtensionInterpCurveMode : uint8
{
	//常规的非线性插值，由快到慢
	Normal UMETA(DisplayName = "常规"),
	//以恒定速率插值
	Constant UMETA(DisplayName = "定速"),
	//插值速度由慢到快的渐进
	FadeIn UMETA(DisplayName = "淡入"),
};

/**
 *这个枚举值代表向量相等的比较类型。
 */
UENUM()
enum class EExtensionVectorCompareMode : uint8
{
	//所有轴
	All UMETA(DisplayName = "常规"),
	//仅X轴
	X UMETA(DisplayName = "仅X轴"),
	//仅Y轴
	Y UMETA(DisplayName = "仅Y轴"),
	//仅Z轴
	Z UMETA(DisplayName = "仅Z轴"),
	//X轴与Y轴
	XY UMETA(DisplayName = "X轴与Y轴"),
	//X轴与Z轴
	XZ UMETA(DisplayName = "X轴与Z轴"),
	//Y轴与Z轴
	YZ UMETA(DisplayName = "Y轴与Z轴"),
};

/**
 *这个枚举值代表旋转体相等的比较类型。
 */
UENUM()
enum class EExtensionRotatorCompareMode : uint8
{
	//所有轴
	All UMETA(DisplayName = "常规"),
	//仅Pitch (Y轴)
	Pitch UMETA(DisplayName = "仅Pitch (Y轴)"),
	//仅Yaw (Z轴)
	Yaw UMETA(DisplayName = "仅Yaw (Z轴)"),
	//仅Roll (X轴)
	Roll UMETA(DisplayName = "仅Roll (X轴)"),
	//Pitch与Yaw (Y轴与Z轴)
	PitchAndYaw UMETA(DisplayName = "Pitch与Yaw (Y轴与Z轴)"),
	//Pitch与Roll (Y轴与X轴)
	PitchAndRoll UMETA(DisplayName = "Pitch与Roll (Y轴与X轴)"),
	//Yaw与Roll (Z轴与X轴)
	YawAndRoll UMETA(DisplayName = "Yaw与Roll (Z轴与X轴)"),
};