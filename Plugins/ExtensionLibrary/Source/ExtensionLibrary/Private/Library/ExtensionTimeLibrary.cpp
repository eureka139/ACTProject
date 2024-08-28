#include "Library/ExtensionTimeLibrary.h"

/////////////////////////////////此处用于初始化静态变量//////////////////////////////////////
TMap<FExtensionTimeTaskInfo, AExtensionTimeTask*> UExtensionTimeLibrary::ExtensionTimeTaskMap;

//////////////////////////////////////////////////////////////////////////
// UExtensionTimeLibrary

void UExtensionTimeLibrary::Delay(TFunction<void()> DelayedFunction, UObject* InObject, float DelayTime)
{
	if (!IsValid(InObject) || !IsValid(InObject->GetWorld())) return;

	//设置定时器
	FTimerHandle DelayTimer;
	FTimerDelegate DelayDelegate;
	DelayDelegate.BindLambda(DelayedFunction);

	//如果延迟时间大于0.0f，则正常设置定时器
	if (DelayTime > 0.0f)
	{
		InObject->GetWorld()->GetTimerManager().SetTimer(DelayTimer, DelayDelegate, DelayTime, false, DelayTime);
	}
	//如果延迟时间小于等于0.0f，则下一帧执行
	else
	{
		InObject->GetWorld()->GetTimerManager().SetTimerForNextTick(DelayDelegate);
	}
	
}

void UExtensionTimeLibrary::SetTimeTask(FExtensionTimeTaskInfo TimeTaskInfo, TFunction<void()> TaskFunction, TFunction<bool()> JudgeFunction,TFunction<void()> FinishFunction, float DeltaTime, float Duration, int TaskTimes, float FirstDelay, bool bCover)
{
	//判断合法性并取得World
	if (!IsValid(TimeTaskInfo.TargetObject) || !IsValid(TimeTaskInfo.TargetObject->GetWorld())) return;
	UWorld* World = TimeTaskInfo.TargetObject->GetWorld();
	
	//先从Map中寻找
	if (ExtensionTimeTaskMap.Contains(TimeTaskInfo))
	{
		//如果已存在该任务并且不接受覆盖，则返回
		if (!bCover) return;

		//如果找到就终止原来的任务
		StopTimeTask(TimeTaskInfo);
	}
	
	//处理不合理的变量
	DeltaTime = DeltaTime < 0.0f ? 0.0f : DeltaTime;
	FirstDelay = FirstDelay < 0.0f ? DeltaTime : FirstDelay;
	
	//创建任务
	AExtensionTimeTask* TimeTask = Cast<AExtensionTimeTask>(World->SpawnActor(AExtensionTimeTask::StaticClass()));
	if (!IsValid(TimeTask)) return;

	//设置参数
	TimeTask->SetTaskParams(TimeTaskInfo, TaskFunction, JudgeFunction, FinishFunction, Duration, TaskTimes);

	//启用Tick
	TimeTask->InitTaskTick(DeltaTime, FirstDelay);
}

void UExtensionTimeLibrary::SetTimeTask(UObject* InObject, FName TaskMark, int TaskUniqueMark, TFunction<void()> TaskFunction,TFunction<bool()> JudgeFunction, TFunction<void()> FinishFunction, float DeltaTime, float Duration, int TaskTimes, float FirstDelay, bool bCover)
{
	//判断合法性并取得World
	if (!IsValid(InObject) || !IsValid(InObject->GetWorld())) return;
	UWorld* World = InObject->GetWorld();

	//处理不合理的变量
	DeltaTime = DeltaTime < 0.0f ? 0.0f : DeltaTime;
	FirstDelay = FirstDelay < 0.0f ? DeltaTime : FirstDelay;
	
	//任务信息
	FExtensionTimeTaskInfo TimeTaskInfo = FExtensionTimeTaskInfo(InObject, TaskMark, TaskUniqueMark);

	//先从Map中寻找
	if (ExtensionTimeTaskMap.Contains(TimeTaskInfo))
	{
		//如果已存在该任务并且不接受覆盖，则返回
		if (!bCover) return;

		//如果找到就终止原来的任务
		StopTimeTask(TimeTaskInfo);
	}
	
	//判断是否应该创建任务
	AExtensionTimeTask* TimeTask = Cast<AExtensionTimeTask>(World->SpawnActor(AExtensionTimeTask::StaticClass()));
	if (!IsValid(TimeTask)) return;

	//设置参数
	TimeTask->SetTaskParams(TimeTaskInfo, TaskFunction, JudgeFunction, FinishFunction, Duration, TaskTimes);

	//启用Tick
	TimeTask->InitTaskTick(DeltaTime,FirstDelay);
}

void UExtensionTimeLibrary::StopTimeTask(FExtensionTimeTaskInfo TimeTaskInfo)
{
	//从Map中寻找
	if (ExtensionTimeTaskMap.Contains(TimeTaskInfo))
	{
		//任务指针
		AExtensionTimeTask* TimeTaskRef = ExtensionTimeTaskMap[TimeTaskInfo];
		if (IsValid(TimeTaskRef))
		{
			TimeTaskRef->DoFinishFunction();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// AExtensionTimeTask

AExtensionTimeTask::AExtensionTimeTask()
{
	PrimaryActorTick.bCanEverTick = true;
	
	//禁用Tick，等待启用
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AExtensionTimeTask::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	//如果可以继续执行，则在Tick中执行任务
	if (JudgeFunction()) DoTaskFunction();
	else DoFinishFunction();
	
	//剩余执行次数为0，则终止,否则次数减一
	if (TaskTimes > 0)
	{
		TaskTimes--;
		if (TaskTimes == 0)
		{
			DoFinishFunction();
			return;
		}
	}
	
	//剩余时长为0，则终止
	if (Duration > 0.0f)
	{
		Duration = FMath::Max(Duration - DeltaSeconds , 0.0f);
		if (Duration == 0.0f)
		{
			DoFinishFunction();
		}
	}
	
}

void AExtensionTimeTask::SetTaskParams(FExtensionTimeTaskInfo InTimeTaskInfo, TFunction<void()> InTaskFunction,TFunction<bool()> InJudgeFunction, TFunction<void()> InFinishFunction, float InDuration, int InTaskTimes)
{
	TimeTaskInfo = InTimeTaskInfo;
	TaskFunction = InTaskFunction;
	JudgeFunction = InJudgeFunction;
	FinishFunction = InFinishFunction;
	Duration = InDuration;
	TaskTimes = InTaskTimes;
}

void AExtensionTimeTask::InitTaskTick(float DeltaTime, float FirstDelay)
{
	//先禁用Tick
	SetActorTickEnabled(false);

	//设置间隔
	SetActorTickInterval(DeltaTime);
	
	//只有在Tick函数没开启时执行
	if (!PrimaryActorTick.IsTickFunctionEnabled())
	{
		//在任务Tick被开启或任务被覆盖时，将该任务存进Map，即使暂停也不会移除，已判断不会重复添加
		if (!UExtensionTimeLibrary::ExtensionTimeTaskMap.Contains(TimeTaskInfo))
		{
			UExtensionTimeLibrary::ExtensionTimeTaskMap.Emplace(TimeTaskInfo, this);
		}

		//如果没有FirstDelay则直接开启
		if (FirstDelay <= 0.0f)
		{
			SetActorTickEnabled(true);
		}
		//如果有FirstDelay则在FirstDelay之后开启
		else
		{
			FTimerHandle FirstDelayTimer;
			FTimerDelegate FirstDelayDelegate;
	
			FirstDelayDelegate.BindLambda([this]
			{
				//由于是延迟执行，无法确定执行这个Lambda函数的时候对象是否还存在，故需要判断
				if (IsValid(this))
				{
					SetActorTickEnabled(true);
				}
			});

			GetWorldTimerManager().SetTimer(FirstDelayTimer,FirstDelayDelegate,FirstDelay,false, FirstDelay);
		}
	}
}

void AExtensionTimeTask::DoTaskFunction()
{
	TaskFunction();
}

void AExtensionTimeTask::DoFinishFunction()
{
	FinishFunction();

	//任务结束时从Map中移除
	UExtensionTimeLibrary::ExtensionTimeTaskMap.Remove(TimeTaskInfo);
	
	Destroy();
}
