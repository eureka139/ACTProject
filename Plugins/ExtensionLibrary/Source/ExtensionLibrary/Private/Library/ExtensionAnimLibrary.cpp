// Copyright Epic Games, Inc. All Rights Reserved.

#include "Library/ExtensionAnimLibrary.h"

//////////////////////////////////////////////////////////////////////////
// UExtensionAnimLibrary

UExtensionMontageProxy* UExtensionAnimLibrary::PlayMontage(
	class USkeletalMeshComponent* InSkeletalMeshComponent,
	class UAnimMontage* MontageToPlay,
	float PlayRate,
	float StartingPosition,
	FName StartingSection)
{
	UExtensionMontageProxy* Proxy = NewObject<UExtensionMontageProxy>();
	Proxy->SetFlags(RF_StrongRefOnFrame);
	Proxy->PlayMontage(InSkeletalMeshComponent, MontageToPlay, PlayRate, StartingPosition, StartingSection);
	return Proxy;
}


//////////////////////////////////////////////////////////////////////////
// UExtensionMontageProxy

UExtensionMontageProxy::UExtensionMontageProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, MontageInstanceID(INDEX_NONE)
	, bInterruptedCalledBeforeBlendingOut(false)
{
}

void UExtensionMontageProxy::PlayMontage(class USkeletalMeshComponent* InSkeletalMeshComponent, 
	class UAnimMontage* MontageToPlay, 
	float PlayRate, 
	float StartingPosition, 
	FName StartingSection)
{
	bool bPlayedSuccessfully = false;
	if (InSkeletalMeshComponent)
	{
		if (UAnimInstance* AnimInstance = InSkeletalMeshComponent->GetAnimInstance())
		{
			const float MontageLength = AnimInstance->Montage_Play(MontageToPlay, PlayRate, EMontagePlayReturnType::MontageLength, StartingPosition);
			bPlayedSuccessfully = (MontageLength > 0.f);

			if (bPlayedSuccessfully)
			{
				AnimInstancePtr = AnimInstance;
				if (FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(MontageToPlay))
				{
					MontageInstanceID = MontageInstance->GetInstanceID();
				}

				if (StartingSection != NAME_None)
				{
					AnimInstance->Montage_JumpToSection(StartingSection, MontageToPlay);
				}

				BlendingOutDelegate.BindUObject(this, &UExtensionMontageProxy::OnMontageBlendingOut);
				AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MontageToPlay);

				MontageEndedDelegate.BindUObject(this, &UExtensionMontageProxy::OnMontageEnded);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);

				AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UExtensionMontageProxy::OnNotifyBeginReceived);
				AnimInstance->OnPlayMontageNotifyEnd.AddDynamic(this, &UExtensionMontageProxy::OnNotifyEndReceived);
			}
		}
	}

	if (!bPlayedSuccessfully)
	{
		OnInterrupted.Broadcast(NAME_None);
	}
}

bool UExtensionMontageProxy::IsNotifyValid(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload) const
{
	return ((MontageInstanceID != INDEX_NONE) && (BranchingPointNotifyPayload.MontageInstanceID == MontageInstanceID));
}


void UExtensionMontageProxy::OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (IsNotifyValid(NotifyName, BranchingPointNotifyPayload))
	{
		OnNotifyBegin.Broadcast(NotifyName);
	}
}


void UExtensionMontageProxy::OnNotifyEndReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (IsNotifyValid(NotifyName, BranchingPointNotifyPayload))
	{
		OnNotifyEnd.Broadcast(NotifyName);
	}
}


void UExtensionMontageProxy::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (bInterrupted)
	{
		OnInterrupted.Broadcast(NAME_None);
		bInterruptedCalledBeforeBlendingOut = true;
	}
	else
	{
		OnBlendOut.Broadcast(NAME_None);
	}
}

void UExtensionMontageProxy::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		OnCompleted.Broadcast(NAME_None);
	}
	else if (!bInterruptedCalledBeforeBlendingOut)
	{
		OnInterrupted.Broadcast(NAME_None);
	}

	UnbindDelegates();
}

void UExtensionMontageProxy::UnbindDelegates()
{
	if (UAnimInstance* AnimInstance = AnimInstancePtr.Get())
	{
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &UExtensionMontageProxy::OnNotifyBeginReceived);
		AnimInstance->OnPlayMontageNotifyEnd.RemoveDynamic(this, &UExtensionMontageProxy::OnNotifyEndReceived);
	}
}

void UExtensionMontageProxy::BeginDestroy()
{
	UnbindDelegates();

	Super::BeginDestroy();
}
