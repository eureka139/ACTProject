#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ACTEnemyCharacter.generated.h"

UCLASS()
class ACT_API AACTEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AACTEnemyCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

protected:



public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDamageActorComponent* EnemyDamageActorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* Cone;

	//受击蒙太奇
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitMontage")
	class UAnimMontage* HitMontage;


};
