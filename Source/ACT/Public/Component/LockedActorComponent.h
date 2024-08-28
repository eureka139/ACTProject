#pragma once

#include "CoreMinimal.h"
#include "ACT/ACTData/ACTEnum.h"
#include "Components/ActorComponent.h"
#include "LockedActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACT_API ULockedActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	ULockedActorComponent();

	friend class AACTCharacter;

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InputLock();

	void GetAllEnemiesAndSortThem();

	UFUNCTION(BlueprintCallable)
	bool ChecklsBlockOrNot(AActor* InActor);

	bool CheckinSightOrNot();

	bool CheckDistance(AActor* InActor);

	float CalculateRotation(AActor* InActor);

	void LockedOn(class AACTEnemyCharacter* InCharacter);

	void LockedOff();

	void TickLocked();

	void LockSwitch(ELockedType InType);

private:

	UPROPERTY()
	class AACTCharacter* ACTCharacter;

	class AACTEnemyCharacter* CurrewntLockedTarget;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AACTEnemyCharacter> ACTEnemyCharacterClass;

	TArray<AActor*>OutActors;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float MaxDistance = 1200.0f;

	TMap<AACTEnemyCharacter*, float>Enemies;

	TArray<float>CenterRotation;

	UPROPERTY(EditAnywhere, Category = "Enum")
	ELockedType LockedType;

protected:



public:	

	bool bIsLocked = false;

		
};
