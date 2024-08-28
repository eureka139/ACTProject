#pragma once

#include "Data/ExtensionLibraryEnum.h"
#include "ExtensionMathLibrary.generated.h"

#define UEML UExtensionMathLibrary

UCLASS()
class EXTENSIONLIBRARY_API UExtensionMathLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 *根据不同的比较模式判断两个向量是否应该被视为相等。
	 *@param Tolerance 容差。当LeftVector与RightVector在对应模式的差值大小在Tolerance之内则视为相同。
	 *@param VectorCompareMode 向量的比较模式，用于选择需要比较的轴。
	 */
	UFUNCTION(BlueprintPure, Category="Vector")
	static bool VectorEqual(FVector LeftVector, FVector RightVector, float Tolerance = 0.0f, EExtensionVectorCompareMode VectorCompareMode = EExtensionVectorCompareMode::All);

	/**
	 *根据不同的比较模式复制向量。 \n
	 *只有选定的轴会被复制。
	 *@param VectorCompareMode 只有VectorCompareMode选定的轴才会被复制。
	 */
	UFUNCTION(BlueprintPure, Category="Vector")
	static FVector VectorCopyByMode(FVector CurrentVector, FVector TargetVector, EExtensionVectorCompareMode VectorCompareMode = EExtensionVectorCompareMode::All);
	
	/**
	 *根据不同的比较模式复制向量。 \n
	 *选定的轴不会被复制。
	 *@param VectorCompareMode VectorCompareMode选定的轴不会被复制。
	 */
	UFUNCTION(BlueprintPure, Category="Vector")
	static FVector VectorCopyExcludedByMode(FVector CurrentVector, FVector TargetVector, EExtensionVectorCompareMode VectorCompareMode = EExtensionVectorCompareMode::All);
	
	/**
	 *对向量进行插值，可根据向量比较模式指定插值的轴。
	 *@param VectorCompareMode 选择需要插值的轴。
	 */
	UFUNCTION(BlueprintPure, Category="Vector")
	static FVector VectorInterpTo(FVector CurrentVector, FVector TargetVector, float DeltaTime, float InterpSpeed, EExtensionVectorCompareMode VectorCompareMode = EExtensionVectorCompareMode::All);

	/**
	 *对向量进行定速插值，可根据向量比较模式指定插值的轴。
	 *@param VectorCompareMode 选择需要插值的轴。
	 */
	UFUNCTION(BlueprintPure, Category="Vector")
	static FVector VectorInterpConstantTo(FVector CurrentVector, FVector TargetVector, float DeltaTime, float InterpSpeed, EExtensionVectorCompareMode VectorCompareMode = EExtensionVectorCompareMode::All);

	/**
	 *根据不同的比较模式判断两个旋转体是否应该被视为相等。
	 *@param Tolerance 容差。当LeftRotator与RightRotator在对应模式的差值大小在Tolerance之内则视为相同。
	 *@param RotatorCompareMode 旋转体的比较模式，用于选择需要比较的轴。
	 */
	UFUNCTION(BlueprintPure, Category="Rotator")
	static bool RotatorEqual(FRotator LeftRotator, FRotator RightRotator, float Tolerance = 0.0f, EExtensionRotatorCompareMode RotatorCompareMode = EExtensionRotatorCompareMode::All);
	
	/**
     *根据不同的比较模式复制旋转体。 \n
	 *只有选定的轴会被复制。
     *@param RotatorCompareMode 只有RotatorCompareMode选定的轴才会被复制。
     */
	UFUNCTION(BlueprintPure, Category="Rotator")
	static FRotator RotatorCopyByMode(FRotator CurrentRotator, FRotator TargetRotator, EExtensionRotatorCompareMode RotatorCompareMode = EExtensionRotatorCompareMode::All);
	
	/**
	 *根据不同的比较模式复制旋转体。 \n
	 *选定的轴不会被复制。
	 *@param RotatorCompareMode RotatorCompareMode选定的轴不会被复制。
	 */
	UFUNCTION(BlueprintPure, Category="Rotator")
	static FRotator RotatorCopyExcludedByMode(FRotator CurrentRotator, FRotator TargetRotator, EExtensionRotatorCompareMode RotatorCompareMode = EExtensionRotatorCompareMode::All);
	
	/**
	 *对旋转体进行插值，可根据旋转体比较模式指定插值的轴。。
	 *@param RotatorCompareMode 选择需要插值的轴。
	 */
	UFUNCTION(BlueprintPure, Category="Rotator")
	static FRotator RotatorInterpTo(FRotator CurrentRotator, FRotator TargetRotator, float DeltaTime, float InterpSpeed, EExtensionRotatorCompareMode RotatorCompareMode = EExtensionRotatorCompareMode::All);

	/**
	 *对旋转体进行定速插值，可根据旋转体比较模式指定插值的轴。
	 *@param RotatorCompareMode 选择需要插值的轴。
	 */
	UFUNCTION(BlueprintPure, Category="Rotator")
	static FRotator RotatorInterpConstantTo(FRotator CurrentRotator, FRotator TargetRotator, float DeltaTime, float InterpSpeed, EExtensionRotatorCompareMode RotatorCompareMode = EExtensionRotatorCompareMode::All);

	/**
	 *获取原始变换在父变换坐标系下的相对变换。
	 *@param OriginalTransform 原始变换。
	 *@param ParentTransform 父变换。
	 */
	UFUNCTION(BlueprintPure, Category="Transform")
	static FTransform TransformOriginalToRelative(FTransform OriginalTransform, FTransform ParentTransform);

	/**
	 *获取父变换坐标系下的相对变换在其原坐标系下的原始变换。
	 *@param RelativeTransform 相对变换。
	 *@param ParentTransform 父变换。
	 */
	UFUNCTION(BlueprintPure, Category="Transform")
	static FTransform TransformRelativeToOriginal(FTransform RelativeTransform, FTransform ParentTransform);

	/**
	 *根据键值选择对应的输出值。
	 *@param InKey 输入的键值。
	 *@param Default 默认的输出值。如果未匹配到InKey，则返回Default。
	 *@param Key 用于匹配并选择的键。
	 *@param Value 如果成功匹配，则返回对应的Value。
	 *@param KeyAndValue 键值和输出值，需要按Key和Value的顺序依次填写。
	 */
	template<typename KeyType, typename ValueType, typename... Args>
	static ValueType Select(KeyType InKey, ValueType Default, KeyType Key, ValueType Value, Args... KeyAndValue);

private:
	/**
	 *Select的递归终点函数。 \n
	 *请不要手动调用。
	 */
	template <typename KeyType, typename ValueType>
	static ValueType Select(KeyType InKey, ValueType Default);
};

template <typename KeyType, typename ValueType>
ValueType UExtensionMathLibrary::Select(KeyType InKey, ValueType Default)
{
	return Default;
}

template <typename KeyType, typename ValueType, typename... Args>
ValueType UExtensionMathLibrary::Select(KeyType InKey, ValueType Default, KeyType Key, ValueType Value, Args... KeyAndValue)
{
	static_assert((sizeof...(Args) % 2) == 0, "UExtensionMathLibrary::Select: 参数的个数必须是偶数");
	
	if (InKey == Key)
	{
		return Value;
	}
	if (sizeof...(KeyAndValue) >= 2)
	{
		return Select(InKey, Default, KeyAndValue...);
	}
	return Default;
}
