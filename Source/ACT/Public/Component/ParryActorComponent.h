#pragma once

#include "CoreMinimal.h"
#include "ACT/ACTData/ACTEnum.h"
#include "Components/ActorComponent.h"
#include "ParryActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACT_API UParryActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UParryActorComponent();

	friend class AACTCharacter;

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ParryStart();

	void ParryEnd();

	void ParrySwitch(FVector AttackDirection,AActor * HitActor);

	void CalculateBlockDirection(FVector AttackDirection, AActor* HitActor);

private:

	UPROPERTY()
	class AACTCharacter* ACTCharacter;



	FName BlockDirectionName;

	UPROPERTY()
	class UExtensionMontageProxy* ParryPlayMontageProxy;

protected:



public:	

	bool bIsParry;

	bool bIsBlockR = true;
};
