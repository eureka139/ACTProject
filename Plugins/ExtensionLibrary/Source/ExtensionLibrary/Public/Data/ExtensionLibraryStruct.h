#pragma once

#include "ExtensionLibraryStruct.generated.h"

struct FExtensionTimeTaskInfo;

//////////////////////////////

/**
 *时间任务的信息。
 */
USTRUCT()
struct FExtensionTimeTaskInfo
{
	GENERATED_BODY()
	
	//发起时间任务的Object
	UPROPERTY()
	UObject* TargetObject;

	//任务的标记，一般是唯一的
	FName TaskMark;

	/**任务的唯一标记，如果为唯一任务，则此标记必然唯一
	 *当任务需要同步调用多个且不需要手动终止的时候，需要生成一个随机整数作为唯一标记
	 */
	int TaskUniqueMark;
	
	//构造函数
	FExtensionTimeTaskInfo() : TargetObject(nullptr), TaskMark(FName()), TaskUniqueMark(0) {}
	FExtensionTimeTaskInfo(UObject* TargetObject, FName TaskMark = FName(), int TaskUniqueMark = 0) : TargetObject(TargetObject), TaskMark(TaskMark), TaskUniqueMark(TaskUniqueMark) {}

	//重载等于号
	bool operator==(const FExtensionTimeTaskInfo& Rhs) const
	{
		return TargetObject == Rhs.TargetObject && TaskMark.IsEqual(Rhs.TaskMark);
	}

	//哈希函数
	friend uint32 GetTypeHash(const FExtensionTimeTaskInfo& TaskInfo)
	{
		uint32 TargetObjectHash = GetTypeHash(TaskInfo.TargetObject);
		uint32 TaskMarkHash = GetTypeHash(TaskInfo.TaskMark);
		uint32 TaskUniqueMarkHash = GetTypeHash(TaskInfo.TaskUniqueMark);
		return HashCombine(TargetObjectHash, TaskMarkHash) + TaskUniqueMarkHash;
	}
	
};