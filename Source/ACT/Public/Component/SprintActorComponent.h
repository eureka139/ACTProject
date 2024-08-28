#pragma once

#include "CoreMinimal.h"
#include "ACT/ACTData/ACTEnum.h"
#include "Components/ActorComponent.h"
#include "SprintActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACT_API USprintActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	USprintActorComponent();

	friend class AACTCharacter;

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Sprint();

	void CalculateAvoidDirection();

private:

	class AACTCharacter* ACTCharacter;

	FName SprintDirection;



protected:



public:	

	UPROPERTY()
	class UExtensionMontageProxy* AvoidPlayMontageProxy;

		
};
