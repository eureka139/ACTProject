#include "Component/LockedActorComponent.h"
#include "Character/ACTCharacter.h"
#include "Character/ACTEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"


ULockedActorComponent::ULockedActorComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

}

void ULockedActorComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void ULockedActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsLocked)
	{
		if (IsValid(CurrewntLockedTarget))
		{
			if (CheckinSightOrNot() && ChecklsBlockOrNot(CurrewntLockedTarget))
			{
				TickLocked();
			}
			else
			{
				LockedOff();
			}
		}
		else
		{
			bIsLocked = false;
		}
	}

}

void ULockedActorComponent::InputLock()
{
	if (!bIsLocked)
	{
		GetAllEnemiesAndSortThem();
		TArray<AACTEnemyCharacter*>EnemiesKeyArray;
		Enemies.GenerateKeyArray(EnemiesKeyArray);
		int32 CenterMinIndex;
		float CenterMinfloat;
		UKismetMathLibrary::MinOfFloatArray(CenterRotation, CenterMinIndex, CenterMinfloat);
		if (CenterMinIndex == -1)
		{
			CenterMinIndex = 0;
		}
		if (IsValid(EnemiesKeyArray[CenterMinIndex]))
		{
			TArray<TEnumAsByte<EObjectTypeQuery>>ObjectType;
			ObjectType = { EObjectTypeQuery::ObjectTypeQuery3 };
			TArray<AActor*>ActorsToIgnore;
			ActorsToIgnore = { EnemiesKeyArray[CenterMinIndex] };
			FHitResult HitResult;
			UKismetSystemLibrary::SphereTraceSingleForObjects(
				GetWorld(),
				ACTCharacter->Camera->GetComponentLocation(),
				EnemiesKeyArray[CenterMinIndex]->GetActorLocation(),
				25.f,
				ObjectType,
				false,
				ActorsToIgnore,
				EDrawDebugTrace::Persistent,
				HitResult,
				true
			);
			if (IsValid(Cast<AACTEnemyCharacter>(HitResult.GetActor())))
			{
				LockedOn(Cast<AACTEnemyCharacter>(HitResult.GetActor()));
			}
			else
			{
				LockedOn(EnemiesKeyArray[CenterMinIndex]);
			}
		}		
	}
	else
	{
		LockedOff();
	}
}

void ULockedActorComponent::GetAllEnemiesAndSortThem()
{
	TMap<AACTEnemyCharacter*, float>LocalEnemies;
	LocalEnemies.Empty();
	CenterRotation.Empty();
	UGameplayStatics::GetAllActorsOfClassWithTag(this, ACTEnemyCharacterClass, FName("Enemy"), OutActors);
	for (int32 Index = 0; Index < OutActors.Num(); Index++) 
	{
		if (ChecklsBlockOrNot(OutActors[Index]) && CheckinSightOrNot())
		{
			LocalEnemies.Add(Cast<AACTEnemyCharacter>(OutActors[Index]), CalculateRotation(OutActors[Index]));
		}
	}
	while (LocalEnemies.Num() > 0)
	{
		TArray<float>FloatArray;
		LocalEnemies.GenerateValueArray(FloatArray);
		TArray<AACTEnemyCharacter*>KeyArray;
		LocalEnemies.GenerateKeyArray(KeyArray);
		int32 MinIndex;
		float MinFloat;
		UKismetMathLibrary::MinOfFloatArray(FloatArray, MinIndex, MinFloat);
		if (MinIndex == -1)
		{
			MinIndex = 0;
		}
		Enemies.Add(KeyArray[MinIndex], MinFloat);
		CenterRotation.Add(UKismetMathLibrary::Abs(MinFloat));
		LocalEnemies.Remove(KeyArray[MinIndex]);
	}
	if (Enemies.Num() > 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, "Enemies.Num() > 0 true");
		return;
	}
}

bool ULockedActorComponent::ChecklsBlockOrNot(AActor* InActor)
{
	TArray<TEnumAsByte<EObjectTypeQuery>>ObjectType;
	ObjectType = { EObjectTypeQuery::ObjectTypeQuery1, EObjectTypeQuery::ObjectTypeQuery2 };
	TArray<AActor*>ActorsToIgnore;
	ActorsToIgnore = { nullptr };
	FHitResult HitResult;
	UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		ACTCharacter->Camera->GetComponentLocation(),
		InActor->GetActorLocation(),
		ObjectType,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true
		);
	return !HitResult.bBlockingHit;
}

bool ULockedActorComponent::CheckinSightOrNot()
{
	TArray<TEnumAsByte<EObjectTypeQuery>>ObjectType;
	ObjectType = { EObjectTypeQuery::ObjectTypeQuery3 };
	TArray<AActor*>ActorsToIgnore;
	ActorsToIgnore = { ACTCharacter };
	FHitResult HitResult;
	UKismetSystemLibrary::SphereTraceSingleForObjects(
		GetWorld(),
		ACTCharacter->Camera->GetComponentLocation(),
		ACTCharacter->Camera->GetComponentLocation() + (ACTCharacter->Camera->GetForwardVector() * 1000.0f),
		400.0f,
		ObjectType,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true
	);
	return 	CheckDistance(HitResult.GetActor());
}

bool ULockedActorComponent::CheckDistance(AActor* InActor)
{
	if (IsValid(InActor))
	{
		return InActor->GetDistanceTo(ACTCharacter) < MaxDistance;
	}
	else
	{
		return false;
	}
}

float ULockedActorComponent::CalculateRotation(AActor* InActor)
{
	return 	UKismetMathLibrary::FindLookAtRotation(ACTCharacter->Camera->GetComponentLocation(), InActor->GetActorLocation()).Yaw - UKismetMathLibrary::FindLookAtRotation(ACTCharacter->Camera->GetComponentLocation(), ACTCharacter->GetActorLocation()).Yaw;
}

void ULockedActorComponent::LockedOn(AACTEnemyCharacter* InCharacter)
{
	CurrewntLockedTarget = InCharacter;
	if (IsValid(CurrewntLockedTarget))
	{
		CurrewntLockedTarget->Cone->SetHiddenInGame(false, false);
		if (IsValid(CurrewntLockedTarget))
		{
			bIsLocked = true;
		}
	}
}

void ULockedActorComponent::LockedOff()
{
	if (IsValid(CurrewntLockedTarget))
	{
		CurrewntLockedTarget->Cone->SetHiddenInGame(true, false);
		bIsLocked = false;
	}
}

void ULockedActorComponent::TickLocked()
{
	UGameplayStatics::GetPlayerController(this, 0)->SetControlRotation(
		FRotator(
			0.f,
			UKismetMathLibrary::FindLookAtRotation(
				UGameplayStatics::GetPlayerController(this, 0)->GetPawn()->GetActorLocation(),
				CurrewntLockedTarget->GetActorLocation()
				).Yaw,
			UGameplayStatics::GetPlayerController(this, 0)->GetControlRotation().Roll));

	
}

void ULockedActorComponent::LockSwitch(ELockedType InType)
{
	if (bIsLocked)
	{
		LockedType = InType;
		GetAllEnemiesAndSortThem();
		if (ChecklsBlockOrNot(CurrewntLockedTarget) && CheckinSightOrNot())
		{
			if (Enemies.Num() > 1)
			{
				TArray<AACTEnemyCharacter*>EnemiesKeyArray;
				Enemies.GenerateKeyArray(EnemiesKeyArray);
				if (LockedType == ELockedType::ELP_LeftLock)
				{
					if (EnemiesKeyArray.Find(CurrewntLockedTarget) == 0)
					{
						TArray<AACTEnemyCharacter*>FirstEnemiesKeyArray;
						Enemies.GenerateKeyArray(FirstEnemiesKeyArray);
						LockedOff();
						if (LockedType == ELockedType::ELP_LeftLock)
						{
							LockedOn(FirstEnemiesKeyArray[FirstEnemiesKeyArray.Find(FirstEnemiesKeyArray.Last())]);
								
						}
						else
						{
							LockedOn(FirstEnemiesKeyArray[0]);
						}
					}
					else
					{
						TArray<AACTEnemyCharacter*>FirstEnemiesKeyArray;
						Enemies.GenerateKeyArray(FirstEnemiesKeyArray);
						LockedOff();
						if (LockedType == ELockedType::ELP_LeftLock)
						{
							LockedOn(FirstEnemiesKeyArray[FirstEnemiesKeyArray.Find(CurrewntLockedTarget) + -1]);
						}
						else
						{
							LockedOn(FirstEnemiesKeyArray[FirstEnemiesKeyArray.Find(CurrewntLockedTarget) + 1]);
						}
					}

				}
				else
				{
					if (EnemiesKeyArray.Find(CurrewntLockedTarget) == EnemiesKeyArray.Find(EnemiesKeyArray.Last()))
					{
						TArray<AACTEnemyCharacter*>FirstEnemiesKeyArray;
						Enemies.GenerateKeyArray(FirstEnemiesKeyArray);
						LockedOff();
						if (LockedType == ELockedType::ELP_LeftLock)
						{
							LockedOn(FirstEnemiesKeyArray[FirstEnemiesKeyArray.Find(FirstEnemiesKeyArray.Last())]);

						}
						else
						{
							LockedOn(FirstEnemiesKeyArray[0]);
						}
					}
					else
					{
						TArray<AACTEnemyCharacter*>FirstEnemiesKeyArray;
						Enemies.GenerateKeyArray(FirstEnemiesKeyArray);
						LockedOff();
						if (LockedType == ELockedType::ELP_LeftLock)
						{
							LockedOn(FirstEnemiesKeyArray[FirstEnemiesKeyArray.Find(CurrewntLockedTarget) + -1]);
						}
						else
						{
							LockedOn(FirstEnemiesKeyArray[FirstEnemiesKeyArray.Find(CurrewntLockedTarget) + 1]);
						}
					}
				}
			}
		}
		else
		{
			LockedOff();
		}
	}
}

