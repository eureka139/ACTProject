#include "Library/ExtensionMathLibrary.h"
#include "Kismet/KismetMathLibrary.h"

bool UExtensionMathLibrary::VectorEqual(FVector LeftVector, FVector RightVector, float Tolerance, EExtensionVectorCompareMode VectorCompareMode)
{
	LeftVector = VectorCopyExcludedByMode(LeftVector, RightVector, VectorCompareMode);
	
	return LeftVector.Equals(RightVector, Tolerance);
}

FVector UExtensionMathLibrary::VectorCopyByMode(FVector CurrentVector, FVector TargetVector, EExtensionVectorCompareMode VectorCompareMode)
{
	//根据旋转体比较模式判断并处理数值
	if (VectorCompareMode == EExtensionVectorCompareMode::X)
	{
		CurrentVector.X = TargetVector.X;
	}
	else if (VectorCompareMode == EExtensionVectorCompareMode::Y)
	{
		CurrentVector.Y = TargetVector.Y;
	}
	else if (VectorCompareMode == EExtensionVectorCompareMode::Z)
	{
		CurrentVector.Z = TargetVector.Z;
	}
	else if (VectorCompareMode == EExtensionVectorCompareMode::XY)
	{
		CurrentVector.X = TargetVector.X;
		CurrentVector.Y = TargetVector.Y;
	}
	else if (VectorCompareMode == EExtensionVectorCompareMode::XZ)
	{
		CurrentVector.X = TargetVector.X;
		CurrentVector.Z = TargetVector.Z;
	}
	else if (VectorCompareMode == EExtensionVectorCompareMode::YZ)
	{
		CurrentVector.Y = TargetVector.Y;
		CurrentVector.Z = TargetVector.Z;
	}

	return CurrentVector;
}

FVector UExtensionMathLibrary::VectorCopyExcludedByMode(FVector CurrentVector, FVector TargetVector, EExtensionVectorCompareMode VectorCompareMode)
{
	//根据比较模式处理数值
	if (VectorCompareMode == EExtensionVectorCompareMode::All)
	{
		TargetVector = CurrentVector;
	}
	else if (VectorCompareMode == EExtensionVectorCompareMode::X)
	{
		TargetVector.X = CurrentVector.X;
	}
	else if (VectorCompareMode == EExtensionVectorCompareMode::Y)
	{
		TargetVector.Y = CurrentVector.Y;
	}
	else if (VectorCompareMode == EExtensionVectorCompareMode::Z)
	{
		TargetVector.Z = CurrentVector.Z;
	}
	else if (VectorCompareMode == EExtensionVectorCompareMode::XY)
	{
		TargetVector.X = CurrentVector.X;
		TargetVector.Y = CurrentVector.Y;
	}
	else if (VectorCompareMode == EExtensionVectorCompareMode::XZ)
	{
		TargetVector.X = CurrentVector.X;
		TargetVector.Z = CurrentVector.Z;
	}
	else if (VectorCompareMode == EExtensionVectorCompareMode::YZ)
	{
		TargetVector.Y = CurrentVector.Y;
		TargetVector.Z = CurrentVector.Z;
	}

	return TargetVector;
}

FVector UExtensionMathLibrary::VectorInterpTo(FVector CurrentVector, FVector TargetVector, float DeltaTime, float InterpSpeed, EExtensionVectorCompareMode VectorCompareMode)
{
	//根据比较模式判断并处理数值
	TargetVector = VectorCopyExcludedByMode(TargetVector, CurrentVector, VectorCompareMode);
	
	return FMath::VInterpTo(CurrentVector, TargetVector, DeltaTime, InterpSpeed);
}

FVector UExtensionMathLibrary::VectorInterpConstantTo(FVector CurrentVector, FVector TargetVector, float DeltaTime, float InterpSpeed, EExtensionVectorCompareMode VectorCompareMode)
{
	//根据比较模式判断并处理数值
	TargetVector = VectorCopyExcludedByMode(TargetVector, CurrentVector, VectorCompareMode);
	
	return FMath::VInterpConstantTo(CurrentVector, TargetVector, DeltaTime, InterpSpeed);
}

bool UExtensionMathLibrary::RotatorEqual(FRotator LeftRotator, FRotator RightRotator, float Tolerance, EExtensionRotatorCompareMode RotatorCompareMode)
{
	LeftRotator = RotatorCopyExcludedByMode(LeftRotator, RightRotator, RotatorCompareMode);
	
	return LeftRotator.Equals(RightRotator, Tolerance);
}

FRotator UExtensionMathLibrary::RotatorCopyByMode(FRotator CurrentRotator, FRotator TargetRotator, EExtensionRotatorCompareMode RotatorCompareMode)
{
	//根据比较模式判断并处理数值
	if (RotatorCompareMode == EExtensionRotatorCompareMode::Pitch)
	{
		CurrentRotator.Pitch = TargetRotator.Pitch;
	}
	else if (RotatorCompareMode == EExtensionRotatorCompareMode::Yaw)
	{
		CurrentRotator.Yaw = TargetRotator.Yaw;
	}
	else if (RotatorCompareMode == EExtensionRotatorCompareMode::Roll)
	{
		CurrentRotator.Roll = TargetRotator.Roll;
	}
	else if (RotatorCompareMode == EExtensionRotatorCompareMode::PitchAndYaw)
	{
		CurrentRotator.Pitch = TargetRotator.Pitch;
		CurrentRotator.Yaw = TargetRotator.Yaw;
	}
	else if (RotatorCompareMode == EExtensionRotatorCompareMode::PitchAndRoll)
	{
		CurrentRotator.Pitch = TargetRotator.Pitch;
		CurrentRotator.Roll = TargetRotator.Roll;
	}
	else if (RotatorCompareMode == EExtensionRotatorCompareMode::YawAndRoll)
	{
		CurrentRotator.Yaw = TargetRotator.Yaw;
		CurrentRotator.Roll = TargetRotator.Roll;
	}

	return CurrentRotator;
}

FRotator UExtensionMathLibrary::RotatorCopyExcludedByMode(FRotator CurrentRotator, FRotator TargetRotator, EExtensionRotatorCompareMode RotatorCompareMode)
{
	//根据比较模式处理数值
	if (RotatorCompareMode == EExtensionRotatorCompareMode::All)
	{
		TargetRotator = CurrentRotator;
	}
	else if (RotatorCompareMode == EExtensionRotatorCompareMode::Pitch)
	{
		TargetRotator.Pitch = CurrentRotator.Pitch;
	}
	else if (RotatorCompareMode == EExtensionRotatorCompareMode::Yaw)
	{
		TargetRotator.Yaw = CurrentRotator.Yaw;
	}
	else if (RotatorCompareMode == EExtensionRotatorCompareMode::Roll)
	{
		TargetRotator.Roll = CurrentRotator.Roll;
	}
	else if (RotatorCompareMode == EExtensionRotatorCompareMode::PitchAndYaw)
	{
		TargetRotator.Pitch = CurrentRotator.Pitch;
		TargetRotator.Yaw = CurrentRotator.Yaw;
	}
	else if (RotatorCompareMode == EExtensionRotatorCompareMode::PitchAndRoll)
	{
		TargetRotator.Pitch = CurrentRotator.Pitch;
		TargetRotator.Roll = CurrentRotator.Roll;
	}
	else if (RotatorCompareMode == EExtensionRotatorCompareMode::YawAndRoll)
	{
		TargetRotator.Yaw = CurrentRotator.Yaw;
		TargetRotator.Roll = CurrentRotator.Roll;
	}

	return TargetRotator;
}

FRotator UExtensionMathLibrary::RotatorInterpTo(FRotator CurrentRotator, FRotator TargetRotator, float DeltaTime, float InterpSpeed, EExtensionRotatorCompareMode RotatorCompareMode)
{
	//根据旋转体比较模式判断并处理数值
	TargetRotator = RotatorCopyExcludedByMode(TargetRotator, CurrentRotator, RotatorCompareMode);

	return FMath::RInterpTo(CurrentRotator, TargetRotator, DeltaTime, InterpSpeed);
}

FRotator UExtensionMathLibrary::RotatorInterpConstantTo(FRotator CurrentRotator, FRotator TargetRotator, float DeltaTime, float InterpSpeed, EExtensionRotatorCompareMode RotatorCompareMode)
{
	//根据旋转体比较模式判断并处理数值
	TargetRotator = RotatorCopyExcludedByMode(TargetRotator, CurrentRotator, RotatorCompareMode);

	return FMath::RInterpConstantTo(CurrentRotator, TargetRotator, DeltaTime, InterpSpeed);
}

FTransform UExtensionMathLibrary::TransformOriginalToRelative(FTransform WorldTransform, FTransform ParentTransform)
{
	return WorldTransform * ParentTransform.Inverse();
}

FTransform UExtensionMathLibrary::TransformRelativeToOriginal(FTransform RelativeTransform, FTransform ParentTransform)
{
	return RelativeTransform * ParentTransform;
}