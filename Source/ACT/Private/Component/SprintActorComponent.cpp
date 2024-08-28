#include "Component/SprintActorComponent.h"
#include "Character/ACTCharacter.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Library/ExtensionAnimLibrary.h"

USprintActorComponent::USprintActorComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

}

void USprintActorComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void USprintActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void USprintActorComponent::Sprint()
{
	if (IsValid(ACTCharacter))
	{
		if (ACTCharacter->ActiveChildIndex == 1)
		{
			switch (ACTCharacter->ACTPlayerState)
			{
			case EPlayerState::EPS_Idle:
				ACTCharacter->ACTPlayerState = EPlayerState::EPS_Avoid;
				CalculateAvoidDirection();
				AvoidPlayMontageProxy = UExtensionAnimLibrary::PlayMontage(ACTCharacter->GetMesh(), ACTCharacter->AvoidMontage, 1.0f, 0.0f, SprintDirection);

				break;

			case EPlayerState::EPS_Move:
				ACTCharacter->ACTPlayerState = EPlayerState::EPS_Avoid;
				CalculateAvoidDirection();
				AvoidPlayMontageProxy = UExtensionAnimLibrary::PlayMontage(ACTCharacter->GetMesh(), ACTCharacter->AvoidMontage, 1.0f, 0.0f, SprintDirection);

				break;

			case EPlayerState::EPS_Attack:
				ACTCharacter->ACTPlayerState = EPlayerState::EPS_Avoid;
				CalculateAvoidDirection();
				AvoidPlayMontageProxy = UExtensionAnimLibrary::PlayMontage(ACTCharacter->GetMesh(), ACTCharacter->AvoidMontage, 1.0f, 0.0f, SprintDirection);

				break;

			case EPlayerState::EPS_ChargeAttack:
				ACTCharacter->ACTPlayerState = EPlayerState::EPS_Avoid;
				CalculateAvoidDirection();
				AvoidPlayMontageProxy = UExtensionAnimLibrary::PlayMontage(ACTCharacter->GetMesh(), ACTCharacter->AvoidMontage, 1.0f, 0.0f, SprintDirection);

				break;
			}
		}
	}

}

void USprintActorComponent::CalculateAvoidDirection()
{
	//X为1时角色向右移动，为-1时角色向左移动
	//Y为1时角色向前移动，为-1时角色向后移动
	if (IsValid(ACTCharacter))
	{
		if (ACTCharacter->MoveAxis.X == 1)
		{
			SprintDirection = FName("Avoid_R");
		}
		else if (ACTCharacter->MoveAxis.X == -1)
		{
			SprintDirection = FName("Avoid_L");
		}
		else if (ACTCharacter->MoveAxis.X == 0)
		{
			if (ACTCharacter->MoveAxis.Y == 1)
			{
				SprintDirection = FName("Avoid_F");
			}
			else if (ACTCharacter->MoveAxis.Y == -1)
			{
				SprintDirection = FName("Avoid_B");
			}
			else
			{
				SprintDirection = FName("Avoid_B");
			}
		}
	}

}

