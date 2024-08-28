#include "Component/BasicActionComponent.h"
#include "Character/ACTCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Library/ExtensionAnimLibrary.h"


UBasicActionComponent::UBasicActionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UBasicActionComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


void UBasicActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UBasicActionComponent::MoveChangeState()
{
	switch (FMath::CeilToInt(MoveState))
	{
	case 0:
		MoveState = 1;
		break;

	case 1:
		MoveState = 0;
		break;

	case 2:
		MoveState = 3;
		break;

	case 3:
		MoveState = 2;
		break;
	}
}

void UBasicActionComponent::Crouching()
{
	switch (FMath::CeilToInt(MoveState))
	{
	case 0:
		switch (ACTCharacter->ACTPlayerState)
		{
		case EPlayerState::EPS_Idle:
			ACTCharacter->ACTPlayerState = EPlayerState::EPS_Crouch;
			MoveState = 2;
			ACTCharacter->Crouch();
			break;
		case EPlayerState::EPS_Move:
			MoveState = 2;
			ACTCharacter->Crouch();
			break;
		}
		break;
	case 1:
		switch (ACTCharacter->ACTPlayerState)
		{
		case EPlayerState::EPS_Idle:
			ACTCharacter->ACTPlayerState = EPlayerState::EPS_Crouch;
			MoveState = 2;
			ACTCharacter->Crouch();
			break;
		case EPlayerState::EPS_Move:
			MoveState = 2;
			ACTCharacter->Crouch();
			break;
		}
		break;
	case 2:
		MoveState = 0;
		ACTCharacter->UnCrouch();
		break;
	}
}

void UBasicActionComponent::JumpStart()
{
	if (MoveState == 0 || MoveState == 1)
	{
		switch (ACTCharacter->ACTPlayerState)
		{
		case EPlayerState::EPS_Idle:
			ACTCharacter->ACTPlayerState = EPlayerState::EPS_Jump;
			ACTCharacter->Jump();
			break;
		case EPlayerState::EPS_Move:
			ACTCharacter->ACTPlayerState = EPlayerState::EPS_Jump;
			ACTCharacter->Jump();
			break;
		}
	}
}

void UBasicActionComponent::JumpEnd()
{
	ACTCharacter->StopJumping();
}

void UBasicActionComponent::Equiped()
{
	switch (ACTCharacter->ACTPlayerState)
	{
	case EPlayerState::EPS_Idle:
		if (ACTCharacter->ActiveChildIndex == 0)
		{
			EquipPlayMontageProxy = UExtensionAnimLibrary::PlayMontage(ACTCharacter->GetMesh(), ACTCharacter->EquipMontage);
			EquipPlayMontageProxy->OnBlendOut.AddDynamic(this, &UBasicActionComponent::EquipeChangeActiveIndex);
			//EquipPlayMontageProxy->OnCompleted.AddDynamic(this, &UBasicActionComponent::EquipeChangeState);
			//EquipPlayMontageProxy->OnInterrupted.AddDynamic(this, &UBasicActionComponent::EquipeChangeState);
		}
		else
		{
			UnarmPlayMontageProxy = UExtensionAnimLibrary::PlayMontage(ACTCharacter->GetMesh(), ACTCharacter->UnarmMontage);
			ACTCharacter->ActiveChildIndex = 0;
			//EquipPlayMontageProxy->OnCompleted.AddDynamic(this, &UBasicActionComponent::EquipeChangeState);
			//EquipPlayMontageProxy->OnInterrupted.AddDynamic(this, &UBasicActionComponent::EquipeChangeState);
		}
		break;
	case EPlayerState::EPS_Move:
		if (ACTCharacter->ActiveChildIndex == 0)
		{
			EquipPlayMontageProxy = UExtensionAnimLibrary::PlayMontage(ACTCharacter->GetMesh(), ACTCharacter->EquipMontage);
			EquipPlayMontageProxy->OnBlendOut.AddDynamic(this, &UBasicActionComponent::EquipeChangeActiveIndex);
			//EquipPlayMontageProxy->OnCompleted.AddDynamic(this, &UBasicActionComponent::EquipeChangeState);
			//EquipPlayMontageProxy->OnInterrupted.AddDynamic(this, &UBasicActionComponent::EquipeChangeState);
		}
		else
		{
			UnarmPlayMontageProxy = UExtensionAnimLibrary::PlayMontage(ACTCharacter->GetMesh(), ACTCharacter->UnarmMontage);
			ACTCharacter->ActiveChildIndex = 0;
			//EquipPlayMontageProxy->OnCompleted.AddDynamic(this, &UBasicActionComponent::EquipeChangeState);
			//EquipPlayMontageProxy->OnInterrupted.AddDynamic(this, &UBasicActionComponent::EquipeChangeState);
		}
		break;
	}
}

void UBasicActionComponent::EquipeChangeState(FName NotifyName)
{
	ACTCharacter->ACTPlayerState = EPlayerState::EPS_Idle;
}

void UBasicActionComponent::EquipeChangeActiveIndex(FName NotifyName)
{
	ACTCharacter->ActiveChildIndex = 1;
}
