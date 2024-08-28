#pragma once

#include "Data/ExtensionLibraryEnum.h"
#include "ExtensionGameplayLibrary.generated.h"

#define UEGL UExtensionGameplayLibrary

UCLASS()
class EXTENSIONLIBRARY_API UExtensionGameplayLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 *获取玩家视口中心的世界位置。 \n
	 *如果InPlayerController无效，将会返回ZeroVector。
	 */
	UFUNCTION(BlueprintCallable, Category="Player Controller", meta=(DefaultToSelf = "InPlayerController"))
	static FVector GetViewportCenterWorldLocation(APlayerController* InPlayerController);
	
	/**
	 *锁定控制旋转至目标。
	 *@param InPlayerController 玩家控制器。
	 *@param TargetActor 锁定的目标Actor。
	 *@param TargetLocation 锁定的目标位置。如果TargetActor无效，则会以TargetLocation进行锁定
	 *@param InterpSpeed 控制旋转插值到目标的速度。
	 *@param DeltaTime 任务执行的时间间隔。若DeltaTime == 0.0f则会每帧执行。
	 *@param Tolerance 容差。当控制旋转与目标旋转的差值大小在Tolerance之内则视为相同。
	 *@param InterpCurveMode 插值曲线模式。
	 *@param RotatorCompareMode 旋转体的比较模式，用于确定实际需要锁定的轴。
	 *@param bFollowInstantlyInTolerance 是否在旋转首次达到容差内之后停止插值，改为瞬时设置旋转。
	 */
	UFUNCTION(BlueprintCallable, Category="Player Controller", meta=(DefaultToSelf = "InPlayerController"))
	static void LockPlayerControllerToTarget(APlayerController* InPlayerController, AActor* TargetActor = nullptr, FVector TargetLocation = FVector::ZeroVector, float InterpSpeed = 25.0f, float DeltaTime = 0.0f, float Tolerance = 0.05f, EExtensionInterpCurveMode InterpCurveMode = EExtensionInterpCurveMode::FadeIn, EExtensionRotatorCompareMode RotatorCompareMode = EExtensionRotatorCompareMode::All, bool bFollowInstantlyInTolerance = true);

	/**
	 *使控制器从目标上解锁。如果控制器并未处于锁定状态，则不会产生效果。
	 */
	UFUNCTION(BlueprintCallable, Category="Player Controller", meta=(DefaultToSelf = "InPlayerController"))
	static void UnlockPlayerControllerFromTarget(APlayerController* InPlayerController);
	
	/**
	 *获取玩家视口中心的世界旋转。 \n
	 *如果InPlayerController无效，将会返回ZeroRotator。
	 */
	UFUNCTION(BlueprintCallable, Category="Viewport", meta=(DefaultToSelf = "InPlayerController"))
	static FRotator GetViewportCenterWorldDirection(APlayerController* InPlayerController);

};