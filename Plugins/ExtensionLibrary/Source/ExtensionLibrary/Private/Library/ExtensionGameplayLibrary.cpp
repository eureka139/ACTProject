#include "Library/ExtensionGameplayLibrary.h"

#include "Kismet/KismetMathLibrary.h"
#include "Library/ExtensionMathLibrary.h"
#include "Library/ExtensionTimeLibrary.h"

FVector UExtensionGameplayLibrary::GetViewportCenterWorldLocation(APlayerController* InPlayerController)
{
	if (!IsValid(InPlayerController))
	{
		UE_LOG(LogTemp, Warning, TEXT("InPlayerController无效，因此返回ZeroVector"));
		return FVector::ZeroVector;
	}
	
	int ViewportSizeX, ViewportSizeY;
	InPlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

	FVector WorldLocation, WorldDirection;
	InPlayerController->DeprojectScreenPositionToWorld(ViewportSizeX * 0.5f, ViewportSizeY * 0.5f, WorldLocation, WorldDirection);

	return WorldLocation;
}

void UExtensionGameplayLibrary::LockPlayerControllerToTarget(APlayerController* InPlayerController, AActor* TargetActor, FVector TargetLocation, float InterpSpeed, float DeltaTime, float Tolerance, EExtensionInterpCurveMode InterpCurveMode, EExtensionRotatorCompareMode RotationCompareMode, bool bFollowInstantlyInTolerance)
{
	if (!IsValid(InPlayerController))
	{
		UE_LOG(LogTemp, Warning, TEXT("InPlayerController无效，因此锁定将不会起作用"));
		return;
	}

	//任务信息
	FExtensionTimeTaskInfo TimeTaskInfo = FExtensionTimeTaskInfo(InPlayerController, FName("Extension.Gameplay.Controller.Lock"));
	
	//移除重复的任务
	UExtensionTimeLibrary::StopTimeTask(TimeTaskInfo);
	
	//处理不合理的变量
	DeltaTime = DeltaTime < 0.0f ? 0.0f : DeltaTime;
	InterpSpeed = InterpSpeed < 0.0f ? 25.0f : InterpSpeed;
	Tolerance = Tolerance < 0.05f ? 0.05f : Tolerance;
	
	//分配函数

	//需要放入Lambda函数的指针
	float* ChangeableSpeed = new float(0.0f);	//可变速度
	bool* bLockInstantly = new bool(false);		//是否瞬时锁定
	
	//任务函数，当TargetActor有效时，控制器锁定至TargetActor，否则锁定在TargetLocation，需要判断并分配不同的任务函数
	TFunction<void()> TaskFunction;;
	if (IsValid(TargetActor))
	{
		TaskFunction = [InPlayerController, TargetActor, DeltaTime, InterpSpeed, Tolerance, InterpCurveMode, RotationCompareMode, bFollowInstantlyInTolerance, ChangeableSpeed, bLockInstantly]
		{
			if (!IsValid(InPlayerController) || !IsValid(TargetActor)) return;
			FVector ViewportCenterLocation = GetViewportCenterWorldLocation(InPlayerController);
			FVector TargetLocation = TargetActor->GetActorLocation();
			FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(ViewportCenterLocation, TargetLocation);
			FRotator InterptedRotation = TargetRotation;

			//检测是否已经到达容差
			bool bRotationInTolerance = UExtensionMathLibrary::RotatorEqual(InPlayerController->GetControlRotation(), TargetRotation, Tolerance, RotationCompareMode);
			
			//如果支持瞬时锁定且首次到达到达容差
			if (!*bLockInstantly && bFollowInstantlyInTolerance)
			{
				*bLockInstantly = bRotationInTolerance;
			}

			//插值
			if (!*bLockInstantly && !bRotationInTolerance)
			{
				if (InterpCurveMode == EExtensionInterpCurveMode::Normal)
				{
					InterptedRotation = UExtensionMathLibrary::RotatorInterpTo(InPlayerController->GetControlRotation(), TargetRotation, DeltaTime == 0.0f ? InPlayerController->GetWorld()->GetDeltaSeconds() : DeltaTime, InterpSpeed, RotationCompareMode);
				}
				else if (InterpCurveMode == EExtensionInterpCurveMode::Constant)
				{
					InterptedRotation = UExtensionMathLibrary::RotatorInterpConstantTo(InPlayerController->GetControlRotation(), TargetRotation, DeltaTime == 0.0f ? InPlayerController->GetWorld()->GetDeltaSeconds() : DeltaTime, InterpSpeed, RotationCompareMode);
				}
				else if (InterpCurveMode == EExtensionInterpCurveMode::FadeIn)
				{
					*ChangeableSpeed = FMath::FInterpTo(*ChangeableSpeed, InterpSpeed, DeltaTime == 0.0f ? InPlayerController->GetWorld()->GetDeltaSeconds() : DeltaTime, InterpSpeed / 10);
					InterptedRotation = UExtensionMathLibrary::RotatorInterpTo(InPlayerController->GetControlRotation(), TargetRotation, DeltaTime == 0.0f ? InPlayerController->GetWorld()->GetDeltaSeconds() : DeltaTime, *ChangeableSpeed, RotationCompareMode);
				}
			}
			else
			{
				InterptedRotation = UExtensionMathLibrary::RotatorCopyByMode(InPlayerController->GetControlRotation(), InterptedRotation, RotationCompareMode);
			}
			
			InPlayerController->SetControlRotation(InterptedRotation);
		};
	}
	else
	{
		TaskFunction = [InPlayerController, TargetLocation, DeltaTime, InterpSpeed, Tolerance, InterpCurveMode, RotationCompareMode, bFollowInstantlyInTolerance, ChangeableSpeed, bLockInstantly]
		{
			if (!IsValid(InPlayerController)) return;
			FVector ViewportCenterLocation = GetViewportCenterWorldLocation(InPlayerController);
			FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(ViewportCenterLocation, TargetLocation);
			FRotator InterptedRotation = TargetRotation;
			
			//检测是否已经到达容差
			bool bRotationInTolerance = UExtensionMathLibrary::RotatorEqual(InPlayerController->GetControlRotation(), TargetRotation, Tolerance, RotationCompareMode);
			
			//如果支持瞬时锁定且首次到达到达容差
			if (!*bLockInstantly && bFollowInstantlyInTolerance)
			{
				*bLockInstantly = bRotationInTolerance;
			}
			
			//插值
			if (!*bLockInstantly && !bRotationInTolerance)
			{
				if (InterpCurveMode == EExtensionInterpCurveMode::Normal)
				{
					InterptedRotation = FMath::RInterpTo(InPlayerController->GetControlRotation(), TargetRotation, DeltaTime == 0.0f ? InPlayerController->GetWorld()->GetDeltaSeconds() : DeltaTime, InterpSpeed);
				}
				else if (InterpCurveMode == EExtensionInterpCurveMode::Constant)
				{
					InterptedRotation = FMath::RInterpConstantTo(InPlayerController->GetControlRotation(), TargetRotation, DeltaTime == 0.0f ? InPlayerController->GetWorld()->GetDeltaSeconds() : DeltaTime, InterpSpeed);
				}
				else if (InterpCurveMode == EExtensionInterpCurveMode::FadeIn)
				{
					*ChangeableSpeed = FMath::FInterpTo(*ChangeableSpeed, InterpSpeed, DeltaTime == 0.0f ? InPlayerController->GetWorld()->GetDeltaSeconds() : DeltaTime, InterpSpeed / 10);
					InterptedRotation = FMath::RInterpTo(InPlayerController->GetControlRotation(), TargetRotation, DeltaTime == 0.0f ? InPlayerController->GetWorld()->GetDeltaSeconds() : DeltaTime, *ChangeableSpeed);
				}
			}
			else
			{
				InterptedRotation = UExtensionMathLibrary::RotatorCopyByMode(InPlayerController->GetControlRotation(), InterptedRotation, RotationCompareMode);
			}
			
			InPlayerController->SetControlRotation(InterptedRotation);
		};
	}

	//判断函数，需要判断TargetActor并分配不同的判断函数
	TFunction<bool()> JudgeFunction;
	if (IsValid(TargetActor))
	{
		JudgeFunction = [InPlayerController, TargetActor]
		{
			return IsValid(InPlayerController) && IsValid(TargetActor);
		};
	}
	else
	{
		JudgeFunction = [InPlayerController]
		{
			return IsValid(InPlayerController);
		};
	}

	//结束函数
	TFunction<void()> FinishFunction = [ChangeableSpeed, bLockInstantly]
	{
		delete ChangeableSpeed;
		delete bLockInstantly;
	};

	//设置任务
	UExtensionTimeLibrary::SetTimeTask(TimeTaskInfo, TaskFunction, JudgeFunction, FinishFunction, DeltaTime, -1.0f, -1, 0.0f, true);
}

void UExtensionGameplayLibrary::UnlockPlayerControllerFromTarget(APlayerController* InPlayerController)
{
	UExtensionTimeLibrary::StopTimeTask(FExtensionTimeTaskInfo(InPlayerController, FName("Extension.Gameplay.Controller.Lock")));
}

FRotator UExtensionGameplayLibrary::GetViewportCenterWorldDirection(APlayerController* InPlayerController)
{
	if (!IsValid(InPlayerController))
	{
		UE_LOG(LogTemp, Warning, TEXT("InPlayerController无效，因此返回ZeroRotator"));
		return FRotator::ZeroRotator;
	}
	
	int ViewportSizeX, ViewportSizeY;
	InPlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

	FVector WorldLocation, WorldDirection;
	InPlayerController->DeprojectScreenPositionToWorld(ViewportSizeX * 0.5f, ViewportSizeY * 0.5f, WorldLocation, WorldDirection);

	return UKismetMathLibrary::MakeRotFromX(WorldDirection);
}
