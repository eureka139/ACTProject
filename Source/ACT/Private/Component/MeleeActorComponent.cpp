#include "Component/MeleeActorComponent.h"
#include "Character/ACTCharacter.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Library/ExtensionAnimLibrary.h"

UMeleeActorComponent::UMeleeActorComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

}

void UMeleeActorComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UMeleeActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UMeleeActorComponent::Attack()
{
	if (ACTCharacter->ActiveChildIndex == 1)
	{
		switch (ACTCharacter->ACTPlayerState)
		{
		case EPlayerState::EPS_Idle:
			ACTCharacter->ACTPlayerState = EPlayerState::EPS_Attack;
			AttackCount = AttackCount + 1;
			if (AttackCount < ACTCharacter->AttackMontage.Num() - 1)
			{
				BeginAttackPlayMontageProxy = UExtensionAnimLibrary::PlayMontage(ACTCharacter->GetMesh(), ACTCharacter->AttackMontage[AttackCount]);
				BeginAttackPlayMontageProxy->OnNotifyBegin.AddDynamic(this, &UMeleeActorComponent::EnableTrace);
				BeginAttackPlayMontageProxy->OnNotifyEnd.AddDynamic(this, &UMeleeActorComponent::DisableTrace);
			}
			else
			{
				LastAttackPlayMontageProxy = UExtensionAnimLibrary::PlayMontage(ACTCharacter->GetMesh(), ACTCharacter->AttackMontage[AttackCount]);
				LastAttackPlayMontageProxy->OnNotifyBegin.AddDynamic(this, &UMeleeActorComponent::EnableTrace);
				LastAttackPlayMontageProxy->OnNotifyEnd.AddDynamic(this, &UMeleeActorComponent::DisableTrace);
				AttackCount = -1;
			}

			break;
		case EPlayerState::EPS_Move:
			ACTCharacter->ACTPlayerState = EPlayerState::EPS_Attack;
			AttackCount = AttackCount + 1;
			if (AttackCount < ACTCharacter->AttackMontage.Num() - 1)
			{
				BeginAttackPlayMontageProxy = UExtensionAnimLibrary::PlayMontage(ACTCharacter->GetMesh(), ACTCharacter->AttackMontage[AttackCount]);
				BeginAttackPlayMontageProxy->OnNotifyBegin.AddDynamic(this, &UMeleeActorComponent::EnableTrace);
				BeginAttackPlayMontageProxy->OnNotifyEnd.AddDynamic(this, &UMeleeActorComponent::DisableTrace);
			}
			else
			{
				LastAttackPlayMontageProxy = UExtensionAnimLibrary::PlayMontage(ACTCharacter->GetMesh(), ACTCharacter->AttackMontage[AttackCount]);
				LastAttackPlayMontageProxy->OnNotifyBegin.AddDynamic(this, &UMeleeActorComponent::EnableTrace);
				LastAttackPlayMontageProxy->OnNotifyEnd.AddDynamic(this, &UMeleeActorComponent::DisableTrace);
				AttackCount = -1;
			}

			break;
		}
	}
}

void UMeleeActorComponent::ChargeAttack()
{
	if (IsValid(ACTCharacter))
	{
		if (ACTCharacter->ActiveChildIndex == 1)
		{
			switch (ACTCharacter->ACTPlayerState)
			{
			case EPlayerState::EPS_Idle:
				ACTCharacter->ACTPlayerState = EPlayerState::EPS_ChargeAttack;
				ChargeAttackPlayMontageProxy = UExtensionAnimLibrary::PlayMontage(ACTCharacter->GetMesh(), ACTCharacter->ChargeAttackMontage, 1.0f, 0.0f, FName("ChargeFirst"));
				ChargeAttackPlayMontageProxy->OnBlendOut.AddDynamic(this, &UMeleeActorComponent::ChargeSuccess);
				//ChargeAttackPlayMontageProxy->OnNotifyEnd.AddDynamic(this, &UMeleeActorComponent::ResetCombo);
				break;

			case EPlayerState::EPS_Move:
				ACTCharacter->ACTPlayerState = EPlayerState::EPS_ChargeAttack;
				ChargeAttackPlayMontageProxy = UExtensionAnimLibrary::PlayMontage(ACTCharacter->GetMesh(), ACTCharacter->ChargeAttackMontage, 1.0f, 0.0f, FName("ChargeFirst"));
				ChargeAttackPlayMontageProxy->OnBlendOut.AddDynamic(this, &UMeleeActorComponent::ChargeSuccess);
				//ChargeAttackPlayMontageProxy->OnNotifyEnd.AddDynamic(this, &UMeleeActorComponent::ResetCombo);
				break;
			}
		}
	}

}

void UMeleeActorComponent::ChargeSuccess(FName NotifyName)
{

	if (ACTCharacter->ACTPlayerState == EPlayerState::EPS_ChargeAttack)
	{
		ACTCharacter->ACTPlayerState = EPlayerState::EPS_ChargeSuccess;
		ChargeSuccessPlayMontageProxy = UExtensionAnimLibrary::PlayMontage(ACTCharacter->GetMesh(), ACTCharacter->ChargeAttackMontage, 1.0f, 0.0f, FName("ChargeLast"));
		ChargeSuccessPlayMontageProxy->OnCompleted.AddDynamic(this, &UMeleeActorComponent::ResetCombo);
		ChargeSuccessPlayMontageProxy->OnInterrupted.AddDynamic(this, &UMeleeActorComponent::ResetCombo);
		ChargeSuccessPlayMontageProxy->OnNotifyBegin.AddDynamic(this, &UMeleeActorComponent::EnableTrace);
		ChargeSuccessPlayMontageProxy->OnNotifyEnd.AddDynamic(this, &UMeleeActorComponent::DisableTrace);
	}
}

void UMeleeActorComponent::EnableTrace(FName NotifyName)
{
	if (IsValid(ACTCharacter))
	{
		ACTCharacter->EnableTrace();
	}
}

void UMeleeActorComponent::DisableTrace(FName NotifyName)
{
	if (IsValid(ACTCharacter))
	{
		ACTCharacter->DisableTrace();
	}
}

void UMeleeActorComponent::ResetCombo(FName NotifyName)
{
	if (IsValid(ACTCharacter))
	{
		ACTCharacter->ResetCombo();
	}
}
