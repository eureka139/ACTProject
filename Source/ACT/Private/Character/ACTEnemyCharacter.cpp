#include "Character/ACTEnemyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Component/DamageActorComponent.h"
#include "Components/StaticMeshComponent.h"

AACTEnemyCharacter::AACTEnemyCharacter()
{

	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	EnemyDamageActorComponent = CreateDefaultSubobject<UDamageActorComponent>(TEXT("EnemyDamageActorComponent"));

	Tags.Add(FName("Enemy"));

	Cone = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cone"));
	Cone->SetupAttachment(GetRootComponent());
	Cone->SetHiddenInGame(true, false);

}

void AACTEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	Cone->SetHiddenInGame(true, false);
	
}

void AACTEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AACTEnemyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (IsValid(EnemyDamageActorComponent))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, "EnemyDamageActorComponent true ");
		EnemyDamageActorComponent->ACTEnemyCharacter = this;
	}
}
