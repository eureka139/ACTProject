#include "Component/DamageActorComponent.h"
#include "Character/ACTCharacter.h"
#include "Character/ACTEnemyCharacter.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Library/ExtensionAnimLibrary.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/EngineTypes.h"
#include "Component/ParryActorComponent.h"

UDamageActorComponent::UDamageActorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UDamageActorComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UDamageActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


void UDamageActorComponent::SaveThePoints()
{
	if (IsValid(ACTCharacter))
	{
		if (IsValid(ACTCharacter->KatanaMesh))
		{
			//从Character身上Katana中获取其所有的子组件,储存到TraceComponent数组中
			ACTCharacter->KatanaMesh->GetChildrenComponents(false, TraceComponent);
			for (int32 Index = 0; Index < TraceComponent.Num(); Index++)
			{
				//将所有获取的子组件储存到同个数组
				TracePoints.Add(TraceComponent[Index]);
				//再储存所有子组件在世界坐标中的位置
				TracePointsLocation.Add(TracePoints[Index]->GetComponentLocation());
			}
		}
	}
}

void UDamageActorComponent::UpdateTracePoints()
{
	for (int32 Index = 0; Index < TracePoints.Num(); Index++)
	{
		TracePointsLocation.SetNum(Index);
		TracePointsLocation.Add(TracePoints[Index]->GetComponentLocation());	
	}
}

void UDamageActorComponent::EnableTrace()
{

	UpdateTracePoints();
	//设置定时器
	GetWorld()->GetTimerManager().SetTimer(
		TraceHandle,
		this,
		&UDamageActorComponent::CreateTrace,
		0.01f,
		true
	);
}

void UDamageActorComponent::CreateTrace()
{
	for (int32 TraceIndex = 0; TraceIndex < TracePoints.Num(); TraceIndex++)
	{
		StartLocation = TracePointsLocation[TraceIndex];
		EndLocation = TracePoints[TraceIndex]->GetComponentLocation();
		//开启射线检测
		UKismetSystemLibrary::SphereTraceMulti(
			GetWorld(),
			StartLocation,
			EndLocation,
			1.0f,
			ETraceTypeQuery::TraceTypeQuery3,
			true,
			ActorsToIgnore,
			EDrawDebugTrace::Persistent,
			OnHits,
			true
			);
		for (int32 HitIndex = 0; HitIndex < OnHits.Num(); HitIndex++)
		{
			if (OnHits[HitIndex].bBlockingHit)
			{
				//储存击中点的数值信息
				HitLocation = OnHits[HitIndex].Location;
				HitActor = OnHits[HitIndex].GetActor();
				HitBoneName = OnHits[HitIndex].BoneName;
				//判断攻击者是否是Enemy
				if (GetOwner()->ActorHasTag(FName("Enemy")))
				{
					//判断受击对象是否是玩家
					if (HitActor->ActorHasTag(FName("Player")))
					{
						//执行玩家受击逻辑
						HitACTCharacter = Cast<AACTCharacter>(HitActor);
						ActorsToIgnore.AddUnique(HitACTCharacter);
						if (HitACTCharacter->ACTPlayerState == EPlayerState::EPS_ParryStart && HitACTCharacter->ACTPlayerState == EPlayerState::EPS_Parrying)
						{
							if (CheckBlockDirection())
							{
								HitACTCharacter->ParryActorComponent->ParrySwitch(UKismetMathLibrary::Normal((StartLocation - EndLocation), 1.0f), Cast<AActor>(HitACTCharacter));
							}
							else
							{
								ACTCharacter->ACTPlayerState = EPlayerState::EPS_Hit;
								HitDirection(HitACTCharacter);
								PlayerPlayMontageProxy = UExtensionAnimLibrary::PlayMontage(HitACTCharacter->GetMesh(), HitACTCharacter->HitMontage, 1.0f, 0.0f, HitDirectionName);
							}
						}
						else
						{
							ACTCharacter->ACTPlayerState = EPlayerState::EPS_Hit;
							HitDirection(HitACTCharacter);
							PlayerPlayMontageProxy = UExtensionAnimLibrary::PlayMontage(HitACTCharacter->GetMesh(), HitACTCharacter->HitMontage, 1.0f, 0.0f, HitDirectionName);
						}
					}
				}
				else
				{
					//判断攻击者是否是玩家
					if (GetOwner()->ActorHasTag(FName("Player")))
					{
						//判断受击对象是否是Enemy
						if (HitActor->ActorHasTag(FName("Enemy")))
						{
							//执行Enemy受击逻辑
							HitACTEnemyCharacter = Cast<AACTEnemyCharacter>(HitActor);
							ActorsToIgnore.AddUnique(HitACTEnemyCharacter);
							HitDirection(HitACTEnemyCharacter);
							EnemyPlayMontageProxy = UExtensionAnimLibrary::PlayMontage(HitACTEnemyCharacter->GetMesh(), HitACTEnemyCharacter->HitMontage,1.0f,0.0f, HitDirectionName);
							RotatorToPlayer(HitACTEnemyCharacter);
						}
					}
				}
			}
		}
	}
	UpdateTracePoints();
}

FVector UDamageActorComponent::CalculateHitDirection(FVector StartLC, FVector EndLC)
{
	return FVector( (EndLC - StartLC).X, (EndLC - StartLC).Y, 0).GetSafeNormal(1.0f);
}

void UDamageActorComponent::RotatorToPlayer(AActor* HitEnemyActor)
{
	HitEnemyActor->SetActorRotation(FRotator(HitEnemyActor->GetActorRotation().Pitch, UKismetMathLibrary::FindLookAtRotation(HitEnemyActor->GetActorLocation(), ACTCharacter->GetActorLocation()).Yaw, HitEnemyActor->GetActorRotation().Roll));
	
}

bool UDamageActorComponent::CheckBlockDirection()
{

	ACTEnemyCharacter = Cast<AACTEnemyCharacter>(GetOwner());
	float BlockDot = UKismetMathLibrary::Dot_VectorVector(
		HitACTCharacter->GetActorForwardVector(),
		UKismetMathLibrary::Normal(
			ACTEnemyCharacter->GetActorLocation() - HitACTCharacter->GetActorLocation(),
			1.0f));
	float BlockCrossZ = UKismetMathLibrary::Cross_VectorVector(
		HitACTCharacter->GetActorForwardVector(),
		UKismetMathLibrary::Normal(
			ACTEnemyCharacter->GetActorLocation() - HitACTCharacter->GetActorLocation(),
			1.0f)).Z;
	return (UKismetMathLibrary::InRange_FloatFloat(BlockDot, 0.3f, 1.0f, true, true) && UKismetMathLibrary::InRange_FloatFloat(BlockCrossZ,-0.4f,0.4f,true,true));
}

void UDamageActorComponent::HitDirection(ACharacter* HitCharacter)
{
	if (IsValid(HitCharacter))
	{
		//点积判断前后方向，叉积判断左右方向，点积大于0则为敌人在前，小于0则为敌人在后，叉积大于0则为敌人在右，小于0则为敌人在左
		//计算受击点与受击者向前向量之间的叉积，若该浮点值大于0，则受击点位于-90 至 90,为前向 ，若该浮点值小于0，则受击点位于90 至 180 至 -180 至 - 90 ，为后向
		//如若要判断精确的正前向受击则需要判断浮点值是否大于0.86（可左右浮动），受击位置在后 ,播放向前倒动画
		if (UKismetMathLibrary::Dot_VectorVector(CalculateHitDirection(StartLocation, EndLocation), HitCharacter->GetActorForwardVector()) > 0.86)
		{
			HitDirectionName = FName("Hit_B");
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, "Hit_B");
		}
		//若浮点值小于-0.7则为受击位置在前，播放向后倒动画
		else if (UKismetMathLibrary::Dot_VectorVector(CalculateHitDirection(StartLocation, EndLocation), HitCharacter->GetActorForwardVector()) < -0.7)
		{
			HitDirectionName = FName("Hit_F");
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, "Hit_F");
		}
		else if (UKismetMathLibrary::Dot_VectorVector(CalculateHitDirection(StartLocation, EndLocation), HitCharacter->GetActorForwardVector()) < -0.5)
		{
			//若点积计算后的浮点值小于-0.5，同时叉积计算后的向量Z值是大于0的，则为受击位置在右前，播放左后倒动画
			if (UKismetMathLibrary::Cross_VectorVector(CalculateHitDirection(StartLocation, EndLocation), HitCharacter->GetActorForwardVector()).Z > 0.f)
			{
				HitDirectionName = FName("Hit_FR");
				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, "Hit_FR");
			}
			//若点积计算后的浮点值大于-0.5，同时叉积计算后的向量Z值是小于0的，则为受击位置在左前，播放向右后倒动画
			else
			{
				HitDirectionName = FName("Hit_FL");
				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, "Hit_FL");
			}
		}
		else if (UKismetMathLibrary::Dot_VectorVector(CalculateHitDirection(StartLocation, EndLocation), HitCharacter->GetActorForwardVector()) > 0.5)
		{
			//若点积计算后的浮点值大于0.5，同时叉积计算后的向量Z值是大于0的，则受击位置在右后，播放向左前倒动画
			if (UKismetMathLibrary::Cross_VectorVector(CalculateHitDirection(StartLocation, EndLocation), HitCharacter->GetActorForwardVector()).Z > 0.f)
			{
				HitDirectionName = FName("Hit_BR");
				//因为没用左前倒动画，所以只能拿前向倒代替
				HitDirectionName = FName("Hit_B");
				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, "Hit_BR");
			}
			//若点积计算后的浮点值小于0.5，同时叉积计算后的向量Z值是小于0的，则受击位置在左后，播放向右前倒动画
			else
			{
				HitDirectionName = FName("Hit_BL");
				//因为没用右前倒动画，所以只能拿前向倒代替
				HitDirectionName = FName("Hit_B");
				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, "Hit_BL");
			}
		}
		else
		{
			//若点积计算结果同时不满足大于0.86，小于-0.7，大于0.5三者，再判断叉积计算结果，若大于0，则受击位置在左，播放向右倒动画，小于0，则受击位置在右，播放向左倒动画
			if (UKismetMathLibrary::Cross_VectorVector(CalculateHitDirection(StartLocation, EndLocation), HitCharacter->GetActorForwardVector()).Z > 0.f)
			{
				HitDirectionName = FName("Hit_R");
				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, "Hit_R");
			}
			else
			{
				HitDirectionName = FName("Hit_L");
				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, "Hit_L");
			}
		}
	}
}

void UDamageActorComponent::DisableTrace()
{
	ActorsToIgnore.Empty();
	GetWorld()->GetTimerManager().ClearTimer(TraceHandle);
}