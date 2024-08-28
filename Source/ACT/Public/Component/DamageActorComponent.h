#pragma once

#include "CoreMinimal.h"
#include "ACT/ACTData/ACTEnum.h"
#include "Components/ActorComponent.h"
#include "DamageActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACT_API UDamageActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UDamageActorComponent();

	friend class AACTCharacter;

	friend class AACTEnemyCharacter;

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//储存检测锚点信息
	void SaveThePoints();
	//实时更新检测锚点的信息
	void UpdateTracePoints();
    //通过蒙太奇通知来启用射线检测
	void EnableTrace();
	//停用射线检测
	void DisableTrace();
	//由定时器启用的射线检测
	UFUNCTION()
	void CreateTrace();
	//计算受击方向
	void HitDirection(ACharacter * HitCharacter);
	//获取受击精准点的位置，并将其规格化
	FVector CalculateHitDirection(FVector StartLC, FVector EndLC);

	void RotatorToPlayer(AActor* HitEnemyActor);

	bool CheckBlockDirection();

private:
	
	UPROPERTY()
	class AACTCharacter* ACTCharacter;
	UPROPERTY()
	class AACTEnemyCharacter* ACTEnemyCharacter;
	//初步获取子组件数组
	TArray<USceneComponent*> TraceComponent;
	//子组件数组
	TArray<USceneComponent*> TracePoints;
	//子组件世界坐标中的位置的数组
	TArray<FVector> TracePointsLocation;
	//定时器句柄
	FTimerHandle TraceHandle;
	//射线检测所需的数据
	FVector StartLocation;
	FVector EndLocation;
	TArray<FHitResult> OnHits;
	TArray<AActor*> ActorsToIgnore;
	//射线检测储存的目标数据
	FVector HitLocation;
	AActor* HitActor;
	FName HitBoneName;
	//玩家受击引用
	UPROPERTY()
	AACTCharacter* HitACTCharacter;
	//敌人受击引用
	UPROPERTY()
	AACTEnemyCharacter* HitACTEnemyCharacter;
	//受击方向命名
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName HitDirectionName;

protected:



public:	

	UPROPERTY()
	class UExtensionMontageProxy* PlayerPlayMontageProxy;

	UPROPERTY()
	UExtensionMontageProxy* EnemyPlayMontageProxy;
		
};
