#pragma once

#include "Animation/AnimInstance.h"
#include "ExtensionAnimLibrary.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMontagePlayDelegate, FName, NotifyName);

#define UEANL UExtensionAnimLibrary

class UExtensionMontageProxy;

UCLASS()
class EXTENSIONLIBRARY_API UExtensionAnimLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/**
	 *播放蒙太奇并返回一个用于绑定代理的UExtensionMontageProxy指针。
	 */
	UFUNCTION(BlueprintCallable)
	static UExtensionMontageProxy* PlayMontage(
		class USkeletalMeshComponent* InSkeletalMeshComponent, 
		class UAnimMontage* MontageToPlay, 
		float PlayRate = 1.f, 
		float StartingPosition = 0.f, 
		FName StartingSection = NAME_None);
};


UCLASS()
class EXTENSIONLIBRARY_API UExtensionMontageProxy : public UObject
{
	GENERATED_UCLASS_BODY()

	friend class UExtensionAnimLibrary;
	
public:
	
	// Called when Montage finished playing and wasn't interrupted
	UPROPERTY(BlueprintAssignable)
	FOnMontagePlayDelegate OnCompleted;

	// Called when Montage starts blending out and is not interrupted
	UPROPERTY(BlueprintAssignable)
	FOnMontagePlayDelegate OnBlendOut;

	// Called when Montage has been interrupted (or failed to play)
	UPROPERTY(BlueprintAssignable)
	FOnMontagePlayDelegate OnInterrupted;

	UPROPERTY(BlueprintAssignable)
	FOnMontagePlayDelegate OnNotifyBegin;

	UPROPERTY(BlueprintAssignable)
	FOnMontagePlayDelegate OnNotifyEnd;

public:
	//~ Begin UObject Interface
	virtual void BeginDestroy() override;
	//~ End UObject Interface

protected:
	UFUNCTION()
	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	UFUNCTION()
	void OnNotifyEndReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

private:
	TWeakObjectPtr<UAnimInstance> AnimInstancePtr;
	int32 MontageInstanceID;
	uint32 bInterruptedCalledBeforeBlendingOut : 1;

	bool IsNotifyValid(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload) const;
	void UnbindDelegates();

	FOnMontageBlendingOutStarted BlendingOutDelegate;
	FOnMontageEnded MontageEndedDelegate;
	
	void PlayMontage(
		class USkeletalMeshComponent* InSkeletalMeshComponent,
		class UAnimMontage* MontageToPlay,
		float PlayRate = 1.f,
		float StartingPosition = 0.f,
		FName StartingSection = NAME_None);
	
};
