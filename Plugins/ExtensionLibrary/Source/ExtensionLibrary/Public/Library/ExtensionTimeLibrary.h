#pragma once

#include "Data/ExtensionLibraryStruct.h"
#include "ExtensionTimeLibrary.generated.h"

#define UETL UExtensionTimeLibrary

class AExtensionTimeTask;

UCLASS()
class EXTENSIONLIBRARY_API UExtensionTimeLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	//静态Map，用于存放所有正在执行任务的对象
	static TMap<FExtensionTimeTaskInfo, AExtensionTimeTask*> ExtensionTimeTaskMap;

public:
	/**
	 *延迟执行。
	 *@param DelayedFunction 需要被延迟执行的函数。
	 *@param InObject 作为代理的UObject。如果InObject的World无效则延迟不会生效。
	 *@param DelayTime 延迟时间。如果DelayTime <= 0.0f，则会在下一帧执行。
	 */
	static void Delay(TFunction<void()> DelayedFunction, UObject* InObject, float DelayTime);
	
	/**
	 *这个函数用于设置与时间相关的任务。 \n
	 *@param TimeTaskInfo 任务信息。
	 *@param TaskFunction 任务的执行主体函数。
	 *@param JudgeFunction 任务的判断函数。只有在返回true时才能执行TaskFunction，当返回false时，任务终止。
	 *@param FinishFunction 任务终止时所调用的函数，一般用于释放内存。
	 *@param DeltaTime 任务执行的时间间隔。若DeltaTime <= 0.0f则会每帧执行。
	 *@param TaskTimes 任务执行的次数。当TaskTimes <= 0时将会无限次调用，直到任务被终止或是Duration从正数归零。
	 *@param Duration 任务执行的次数。Duration <= 0.0f时将会无限次调用，直到任务被终止或是TaskTimes从正数归零。
	 *@param FirstDelay 在延迟FirstDelay之后才首次执行任务。当FirstDelay < 0.0f，则会使用Deltatime作为FirstDelay。
	 *@param bCover 是否接受覆盖。若接受覆盖，则之前未执行完的相同任务会被终止，当前任务会作为替代。若不接受覆盖，如果已有正在执行的相同任务，本次执行不会有效果。
	 */
	static void SetTimeTask(FExtensionTimeTaskInfo TimeTaskInfo, TFunction<void()> TaskFunction, TFunction<bool()> JudgeFunction = [] { return true; }, TFunction<void()> FinishFunction = [] {}, float DeltaTime = 0.0f, float Duration = -1.0f, int TaskTimes = -1, float FirstDelay = -1.0f, bool bCover = true);

	/**
	 *这个函数用于设置与时间相关的任务。 \n
	 *@param InObject 任务的执行对象。
	 *@param TaskMark 任务的标记，用于区分相同对象的不同任务。
	 *@param TaskUniqueMark 任务的唯一标记，用于区分相同任务的不同执行体。
	 *@param TaskFunction 任务的执行主体函数。
	 *@param JudgeFunction 任务的判断函数。只有在返回true时才能执行TaskFunction，当返回false时，任务终止。
	 *@param FinishFunction 任务终止时所调用的函数，一般用于释放内存。
	 *@param DeltaTime 任务执行的时间间隔。若DeltaTime <= 0.0f则会每帧执行。
	 *@param TaskTimes 任务执行的次数。当TaskTimes <= 0时将会无限次调用，直到任务被终止或是Duration从正数归零。
	 *@param Duration 任务执行的次数。Duration <= 0.0f时将会无限次调用，直到任务被终止或是TaskTimesc从正数归零。
	 *@param FirstDelay 在延迟FirstDelay之后才首次执行任务。当FirstDelay < 0.0f，则会使用Deltatime作为FirstDelay。
	 *@param bCover 是否接受覆盖。若接受覆盖，则之前未执行完的相同任务会被终止，当前任务会作为替代。若不接受覆盖，如果已有正在执行的相同任务，本次执行不会有效果。
	 */
	static void SetTimeTask(UObject* InObject, FName TaskMark, int TaskUniqueMark, TFunction<void()> TaskFunction, TFunction<bool()> JudgeFunction = [] { return true; }, TFunction<void()> FinishFunction = [] {}, float DeltaTime = 0.0f, float Duration = -1.0f, int TaskTimes = -1, float FirstDelay = -1.0f, bool bCover = true);

	/**
	 *使用任务信息移除正在执行的任务。 \n
	 *一般情况下不需要手动调用这个函数。
	 */
	UFUNCTION()
	static void StopTimeTask(FExtensionTimeTaskInfo TimeTaskInfo);
};

/**
 *这个类用于执行与时间相关的任务。 \n
 *请不要手动生成这个类的对象。
 */
UCLASS()
class EXTENSIONLIBRARY_API AExtensionTimeTask : public AActor
{
	GENERATED_BODY()

public:
	AExtensionTimeTask();

	virtual void Tick(float DeltaSeconds) override;

	//设置任务的有关参数
	void SetTaskParams(FExtensionTimeTaskInfo InTimeTaskInfo, TFunction<void()> InTaskFunction, TFunction<bool()> InJudgeFunction,TFunction<void()> InFinishFunction, float InDuration = -1.0f, int InTaskTimes = -1);
	
	//初始化并设置启用Tick，如果正在Tick，则单独更改DeltaTime
	void InitTaskTick(float DeltaTime = 0.0f, float FirstDelay = 0.0f);

	//执行任务的函数，在需要执行任务的时候调用
	void DoTaskFunction();

	//结束任务时执行的函数，在任务执行结束或应该走完生命周期时调用
	void DoFinishFunction();

public:
	
protected:
	/**
	 *任务的持续时间，默认为无限时长。
	 *Duration < 0.0f，代表这个任务将无限执行，直到被手动终止、判断函数返回false或是TaskTimes归零。
	 */
	float Duration = -1.0f;
	
	/**
	 *任务执行次数，默认为无限循环。
	 *当TaskTimes == -1，代表这个任务将无限循环，直到被手动终止、判断函数返回false或是Duration归零。
	 */
	int TaskTimes = -1;

	//时间管理器所需执行的任务函数，通过Lambda传递
	TFunction<void()> TaskFunction = [] {};

	//用于判断执行条件的表达式,返回true才执行
	TFunction<bool()> JudgeFunction = [] { return true; };
	
	//在任务执行结束或应该走完生命周期时调用
	TFunction<void()> FinishFunction = [] {};

	//任务信息，用于区分任务
	FExtensionTimeTaskInfo TimeTaskInfo;
};
