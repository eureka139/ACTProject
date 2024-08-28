#include "Library/ExtensionActorLibrary.h"
#include "Library/ExtensionMathLibrary.h"
#include "Library/ExtensionTimeLibrary.h"

void UExtensionActorLibrary::InterpActorLocation(AActor* TargetActor, FVector TargetLocation, float InterpSpeed, float DeltaTime, float Tolerance, EExtensionInterpCurveMode InterpCurveMode, EExtensionVectorCompareMode VectorCompareMode, bool bSweep, bool bCover)
{
	//TargetActor无效或者插值速度不合理则返回
	if (!IsValid(TargetActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetActor无效，因此本次插值将不会起作用"));
		return;
	}
	if (InterpSpeed <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("InterpSpeed并非大于0.0f，因此本次插值将不会起作用"));
		return;
	}
	
	//当前任务的任务标记
	FName TaskMark = FName("Extension.Actor.Interp.Location");

	//任务信息
	FExtensionTimeTaskInfo TimeTaskInfo = FExtensionTimeTaskInfo(TargetActor, TaskMark);

	//移除重复的任务
	UExtensionTimeLibrary::StopTimeTask(TimeTaskInfo);
	
	//处理一些不合理的变量
	DeltaTime = DeltaTime < 0.0f ? 0.0f : DeltaTime;
	Tolerance = Tolerance < 0.000001f ? 0.000001f : Tolerance;
	
	//可能需要可变速度，如使用FadeIn插值
	float* ChangeableInterpSpeed = new float(0.0f);
	
	//分配函数，可能是新的，也可能是重新分配

	//任务执行函数
	TFunction<void()> TaskFunction = [TargetActor, TargetLocation, InterpSpeed, DeltaTime, InterpCurveMode, VectorCompareMode, bSweep, TaskMark, ChangeableInterpSpeed]
	{
		if (!IsValid(TargetActor)) return;

		//按照不同的情况执行插值
		FVector InterpedLocation;
		//与位置相关的插值需#include ""#include ""Library/ExtensionGameplayLibrary.要判断bSweep，由于实际判断需要插值并设置位置，所以记录插值前的Location，在检测到碰撞后设置回原位
		FVector FormerLocation = TargetActor->GetActorLocation();

		//按插值曲线分配函数
		if (InterpCurveMode == EExtensionInterpCurveMode::Normal)
		{
			InterpedLocation = UExtensionMathLibrary::VectorInterpTo(TargetActor->GetActorLocation(), TargetLocation, DeltaTime == 0.0f ? TargetActor->GetWorld()->GetDeltaSeconds() : DeltaTime, InterpSpeed, VectorCompareMode);
		}
		else if (InterpCurveMode == EExtensionInterpCurveMode::Constant)
		{
			InterpedLocation = UExtensionMathLibrary::VectorInterpConstantTo(TargetActor->GetActorLocation(), TargetLocation, DeltaTime == 0.0f ? TargetActor->GetWorld()->GetDeltaSeconds() : DeltaTime, InterpSpeed, VectorCompareMode);
		}
		else if (InterpCurveMode == EExtensionInterpCurveMode::FadeIn)
		{
			*ChangeableInterpSpeed = FMath::FInterpTo(*ChangeableInterpSpeed, InterpSpeed, DeltaTime == 0.0f ? TargetActor->GetWorld()->GetDeltaSeconds() : DeltaTime,  InterpSpeed / 10);
			InterpedLocation = UExtensionMathLibrary::VectorInterpTo(TargetActor->GetActorLocation(), TargetLocation, DeltaTime == 0.0f ? TargetActor->GetWorld()->GetDeltaSeconds() : DeltaTime, *ChangeableInterpSpeed, VectorCompareMode);
		}
		
		FHitResult HitResult;
		//设置Location
		TargetActor->SetActorLocation(InterpedLocation, bSweep, &HitResult);
		if (bSweep && HitResult.bBlockingHit)
		{
			//检测到碰撞后设置回原位
			TargetActor->SetActorLocation(FormerLocation);
			//停止目标插值
			StopInterpActorLocation(TargetActor);
		}
	};

	//判断函数
	TFunction<bool()> JudgeFunction = [TargetActor, TargetLocation, Tolerance, VectorCompareMode]
	{
		//TargetActor已经无效或是TargetActor的插值已经到达容差范围内
		if (!IsValid(TargetActor) || UExtensionMathLibrary::VectorEqual(TargetActor->GetActorLocation(),TargetLocation, Tolerance, VectorCompareMode))
		{
			return false;
		}
		return true;
	};
	
	//结束函数
	TFunction<void()> FinishFunction = [TargetActor, ChangeableInterpSpeed]
	{
		if (TargetActor)
		{
			//删除ChangeableInterpSpeed指针
			delete ChangeableInterpSpeed;
		}
	};

	UExtensionTimeLibrary::SetTimeTask(TimeTaskInfo, TaskFunction, JudgeFunction, FinishFunction, DeltaTime, -1.0f, -1, 0.0f, bCover);
}

void UExtensionActorLibrary::StopInterpActorLocation(AActor* TargetActor)
{
	if (!IsValid(TargetActor)) return;
	UExtensionTimeLibrary::StopTimeTask(FExtensionTimeTaskInfo(TargetActor, FName("Extension.Actor.Interp.Location")));
}

void UExtensionActorLibrary::InterpActorRotation(AActor* TargetActor, FRotator TargetRotation, float InterpSpeed,float DeltaTime, float Tolerance, EExtensionInterpCurveMode InterpCurveMode, EExtensionRotatorCompareMode RotatorCompareMode, bool bCover)
{
	//TargetActor无效或者插值速度不合理则返回
	if (!IsValid(TargetActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetActor无效，因此本次插值将不会起作用"));
		return;
	}
	if (InterpSpeed <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("InterpSpeed并非大于0.0f，因此本次插值将不会起作用"));
		return;
	}
	
	//当前任务的任务标记
	FName TaskMark = FName("Extension.Actor.Interp.Rotation");
	
	//任务信息
	FExtensionTimeTaskInfo TimeTaskInfo = FExtensionTimeTaskInfo(TargetActor, TaskMark);

	//移除重复的任务
	UExtensionTimeLibrary::StopTimeTask(TimeTaskInfo);
	
	//处理一些不合理的变量，在此处Tolerance需要大于等于0.000001f
	DeltaTime = DeltaTime < 0.0f ? 0.0f : DeltaTime;
	Tolerance = Tolerance < 0.000001f ? 0.000001f : Tolerance;
	
	//可能需要可变速度，如使用FadeIn插值
	float* ChangeableInterpSpeed = new float(0.0f);
	
	//分配函数，可能是新的，也可能是重新分配

	//任务执行函数
	TFunction<void()> TaskFunction = [TargetActor, TargetRotation, InterpSpeed, DeltaTime, InterpCurveMode, RotatorCompareMode, ChangeableInterpSpeed]
	{
		if (!IsValid(TargetActor)) return;

		//按照不同的情况执行插值
		FRotator InterpedRotation;

		//按插值曲线分配函数
		if (InterpCurveMode == EExtensionInterpCurveMode::Normal)
		{
			InterpedRotation = UExtensionMathLibrary::RotatorInterpTo(TargetActor->GetActorRotation(), TargetRotation, DeltaTime == 0.0f ? TargetActor->GetWorld()->GetDeltaSeconds() : DeltaTime, InterpSpeed, RotatorCompareMode);
		}
		else if (InterpCurveMode == EExtensionInterpCurveMode::Constant)
		{
			InterpedRotation = UExtensionMathLibrary::RotatorInterpConstantTo(TargetActor->GetActorRotation(), TargetRotation, DeltaTime == 0.0f ? TargetActor->GetWorld()->GetDeltaSeconds() : DeltaTime, InterpSpeed, RotatorCompareMode);
		}
		else if (InterpCurveMode == EExtensionInterpCurveMode::FadeIn)
		{
			*ChangeableInterpSpeed = FMath::FInterpTo(*ChangeableInterpSpeed, InterpSpeed, DeltaTime == 0.0f ? TargetActor->GetWorld()->GetDeltaSeconds() : DeltaTime, InterpSpeed / 10);
			InterpedRotation = UExtensionMathLibrary::RotatorInterpTo(TargetActor->GetActorRotation(), TargetRotation, DeltaTime == 0.0f ? TargetActor->GetWorld()->GetDeltaSeconds() : DeltaTime, *ChangeableInterpSpeed, RotatorCompareMode);
		}
		
		//插值后的数值
		TargetActor->SetActorRotation(InterpedRotation);
	};

	//判断函数
	TFunction<bool()> JudgeFunction = [TargetActor, TargetRotation, Tolerance, RotatorCompareMode]
	{
		//TargetActor已经无效或是TargetActor的插值已经到达容差范围内
		if (!IsValid(TargetActor) || UExtensionMathLibrary::RotatorEqual(TargetActor->GetActorRotation(), TargetRotation, Tolerance, RotatorCompareMode))
		{
			return false;
		}
		return true;
	};
	
	//设置结束函数
	TFunction<void()> FinishFunction = [TargetActor, ChangeableInterpSpeed]
	{
		if (TargetActor)
		{
			//删除ChangeableInterpSpeed指针
			delete ChangeableInterpSpeed;
		}
	};
	
	//新建任务
	UExtensionTimeLibrary::SetTimeTask(TimeTaskInfo, TaskFunction, JudgeFunction, FinishFunction, DeltaTime, -1.0f, -1, 0.0f, bCover);
}

void UExtensionActorLibrary::StopInterpActorRotation(AActor* TargetActor)
{
	if (!IsValid(TargetActor)) return;
	UExtensionTimeLibrary::StopTimeTask(FExtensionTimeTaskInfo(TargetActor, FName("Extension.Actor.Interp.Rotation")));
}
