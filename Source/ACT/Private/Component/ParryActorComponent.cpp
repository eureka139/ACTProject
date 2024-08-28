#include "Component/ParryActorComponent.h"
#include "Character/ACTCharacter.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Library/ExtensionAnimLibrary.h"
#include "Kismet/KismetMathLibrary.h"

UParryActorComponent::UParryActorComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

}

void UParryActorComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UParryActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UParryActorComponent::ParryStart()
{
	if (ACTCharacter->ActiveChildIndex == 1)
	{
		switch (ACTCharacter->ACTPlayerState)
		{
		case EPlayerState::EPS_Idle:
			bIsParry = true;
			break;

		case EPlayerState::EPS_Move:
			bIsParry = true;
			break;
		}
	}
}

void UParryActorComponent::ParryEnd()
{
	if (ACTCharacter->ActiveChildIndex == 1)
	{
		bIsParry = false;
	}
}

void UParryActorComponent::ParrySwitch(FVector AttackDirection, AActor* HitActor)
{
	CalculateBlockDirection(AttackDirection, HitActor);
	AACTCharacter* ParryCharacter = Cast<AACTCharacter>(HitActor);
	ParryPlayMontageProxy = UExtensionAnimLibrary::PlayMontage(ParryCharacter->GetMesh(), ParryCharacter->ParryMontage,1.0f,0.0f, BlockDirectionName);
}

void UParryActorComponent::CalculateBlockDirection(FVector AttackDirection, AActor* HitActor)
{
	float LocalCrossX = UKismetMathLibrary::Cross_VectorVector(AttackDirection, HitActor->GetActorForwardVector()).X;
	float LocalCrossZ = UKismetMathLibrary::Cross_VectorVector(AttackDirection, HitActor->GetActorForwardVector()).Z;
	float LocalDot = UKismetMathLibrary::Dot_VectorVector(AttackDirection, HitActor->GetActorUpVector());

	/*
	* 针对自上而下或者自下而上的攻击，可以近似的看出在YZ面内的叉乘，那么只需要判断结果><0即可
	* Corrs的X值>0代表自下而上打，<0代表自上而下打
	* 
	* 针对格挡U2D或者是L-U2D-R
	* 可以使用Actor向上的向量和攻击反方向进行Dot判断
    * Dot的值(=1)越大，越接近垂直攻击
	* 
	* 同理，对于自左向右或者自右向左的攻击，可以近似看作XY平面内的叉乘
	* Cross的Z值>0从左向右打，<0从右向左打
	*/

	if (bIsBlockR)
	{
		//Corrs的X值>0代表自下而上打，<0代表自上而下打
		if (LocalCrossX <= 0)
		{
			//Cross的Z值>0从左向右打，<0从右向左打
			if (LocalCrossZ > 0)
			{
				//Dot的值(=1)越大，越接近垂直攻击
				if (LocalDot > 0.9)
				{
					//垂直受击
					BlockDirectionName = FName("Parry_R_U2");
				}
				else
				{
					//斜着受击
					BlockDirectionName = FName("Parry_R_U1");
				}
			}
			else
			{
				BlockDirectionName = FName("Parry_R2L_U");
				bIsBlockR = false;
			}
		}
		else
		{
			//Cross的Z值>0从左向右打，<0从右向左打
			if (LocalCrossZ >= 0)
			{
				BlockDirectionName = FName("Parry_R_D");
			}
			else
			{
				BlockDirectionName = FName("Parry_R2L_D");
				bIsBlockR = false;
			}
		}
	}
	else
	{
		//Corrs的X值>0代表自下而上打，<0代表自上而下打
		if (LocalCrossX <= 0)
		{
			//Cross的Z值>0从左向右打，<0从右向左打
			if (LocalCrossZ >= 0)
			{
				BlockDirectionName = FName("Parry_L2R_U");
				bIsBlockR = true;
			}
			else
			{
				//Dot的值(=1)越大，越接近垂直攻击
				if (LocalDot > 0.9)
				{
					BlockDirectionName = FName("Parry_L_U2");
				}
				else
				{
					BlockDirectionName = FName("Parry_L_U1");
				}
			}
		}
		else
		{
			//Cross的Z值>0从左向右打，<0从右向左打
			if (LocalCrossZ >= 0)
			{
				BlockDirectionName = FName("Parry_L2R_D");
				bIsBlockR = true;
			}
			else
			{
				BlockDirectionName = FName("Parry_L_D");
			}
		}
	}
}

