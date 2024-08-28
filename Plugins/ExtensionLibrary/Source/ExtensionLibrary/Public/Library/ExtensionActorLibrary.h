#pragma once

#include "Data/ExtensionLibraryEnum.h"
#include "ExtensionActorLibrary.generated.h"

#define UEACL UExtensionActorLibrary

UCLASS()
class EXTENSIONLIBRARY_API UExtensionActorLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 *这个函数用于插值Actor的Location。 \n
	 *一旦这个函数被调用，Location将会一直保持插值状态，直到被同个插值覆盖或被停止。
	 *@param InterpSpeed 插值速度。如果是Constant插值，则该速度需设为世界速度(绝对速度)。当InterpSpeed <= 0.0f时插值不会生效。
	 *@param DeltaTime 间隔时间，若其数值小于等于0.0f，则会每帧插值。不建议设置过大，否则可能会导致瞬移。
	 *@param Tolerance 容差，当Actor的实际位置与目标位置的距离小于Tolerance时，插值会自动解除。这个值不会小于0.000001.f。该Tolerance最好设置在0.05以上。
	 *@param InterpCurveMode 插值的曲线类型，默认为普通非线性插值。
	 *@param VectorCompareMode 决定向量哪些轴需要参与插值。
	 *@param bSweep 是否接受碰撞扫描。若启用，则Actor一旦发生碰撞，会立刻停止位置插值。注意，碰撞扫描仅对根组件有效。
	 *@param bCover 本次执行是否覆盖原有的Location插值。若覆盖，则当前Actor将立刻开始向新的Location进行插值，并且覆盖所有参数。反之则本次执行不会产生效果.
	 */
	UFUNCTION(BlueprintCallable, Category="Actor", meta=(DefaultToSelf = "TargetActor"))
	static void InterpActorLocation(AActor* TargetActor, FVector TargetLocation, float InterpSpeed, float DeltaTime = 0.0f, float Tolerance = 0.0f, EExtensionInterpCurveMode InterpCurveMode = EExtensionInterpCurveMode::Normal, EExtensionVectorCompareMode VectorCompareMode = EExtensionVectorCompareMode::All, bool bSweep = false, bool bCover = true);

	/**
	 *停止Actor的Location插值。 \n
	 *如果目标Actor并没有在插值Location，则不会有效果。
	 */
	UFUNCTION(BlueprintCallable, Category="Actor", meta=(DefaultToSelf = "TargetActor"))
	static void StopInterpActorLocation(AActor* TargetActor);

	/**
	 *这个函数用于插值Actor的Rotation。 \n
	 *一旦这个函数被调用，Location将会一直保持插值状态，直到被同个插值覆盖或被停止。
	 *@param InterpSpeed 插值速度。如果是Constant插值，则该速度需设为世界速度(绝对速度)。当InterpSpeed <= 0.0f时插值不会生效。
	 *@param DeltaTime 间隔时间，若其数值小于等于0.0f，则会每帧插值。
	 *@param Tolerance 容差，当Actor的实际旋转与目标旋转的角度差值小于Tolerance时，插值会自动解除。这个值不会小于0.000001.f。该Tolerance最好设置在0.05以上。
	 *@param InterpCurveMode 插值曲线模式，默认为普通非线性插值。
	 *@param RotatorCompareMode 决定旋转的哪些轴会参与插值。
	 *@param bCover 本次执行是否覆盖原有的Rotation插值。若覆盖，则当前Actor将立刻开始向新的Rotation进行插值，并且覆盖所有参数。反之则本次执行不会产生效果.
	 */
	UFUNCTION(BlueprintCallable, Category="Actor", meta=(DefaultToSelf = "TargetActor"))
	static void InterpActorRotation(AActor* TargetActor, FRotator TargetRotation, float InterpSpeed, float DeltaTime = 0.0f, float Tolerance = 0.000001f,EExtensionInterpCurveMode InterpCurveMode = EExtensionInterpCurveMode::Normal, EExtensionRotatorCompareMode RotatorCompareMode = EExtensionRotatorCompareMode::All, bool bCover = true);
	
	/**
	 *停止Actor的Rotaion插值。 \n
	 *如果目标Actor并没有在插值Rotaion，则不会有效果。
	 */
	UFUNCTION(BlueprintCallable, Category="Actor", meta=(DefaultToSelf = "TargetActor"))
	static void StopInterpActorRotation(AActor* TargetActor);

};
