#include "Character/ACTCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputLibrary.h"
#include "EnhancedInputComponent.h"
#include "Component/BasicActionComponent.h"
#include "Component/MeleeActorComponent.h"
#include "Component/DamageActorComponent.h"
#include "Component/SprintActorComponent.h"
#include "Component/LockedActorComponent.h"
#include "Component/ParryActorComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "Interface/AnimChangeStateInterface.h"

AACTCharacter::AACTCharacter()
{

	PrimaryActorTick.bCanEverTick = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetMesh());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	KatanaMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KatanaMesh"));
	KatanaMesh->SetupAttachment(GetMesh(),FName("katana_r"));

	ScabbardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ScabbardMesh"));
	ScabbardMesh->SetupAttachment(GetMesh(), FName("Scabbard_Target01"));

	TraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("TraceEnd"));
	TraceEnd->SetupAttachment(KatanaMesh, FName("TraceEnd"));

	Trace3 = CreateDefaultSubobject<USceneComponent>(TEXT("Trace3"));
	Trace3->SetupAttachment(KatanaMesh, FName("Trace3"));

	Trace2 = CreateDefaultSubobject<USceneComponent>(TEXT("Trace2"));
	Trace2->SetupAttachment(KatanaMesh, FName("Trace2"));

	Trace1 = CreateDefaultSubobject<USceneComponent>(TEXT("Trace1"));
	Trace1->SetupAttachment(KatanaMesh, FName("Trace1"));

	TraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("TraceStart"));
	TraceStart->SetupAttachment(KatanaMesh, FName("TraceStart"));

	BasicActionComponent = CreateDefaultSubobject<UBasicActionComponent>(TEXT("BasicActionComponent"));
	MeleeActionComponent = CreateDefaultSubobject<UMeleeActorComponent>(TEXT("MeleeActionComponent"));
	DamageActorComponent = CreateDefaultSubobject<UDamageActorComponent>(TEXT("DamageActorComponent"));
	SprintActorComponent = CreateDefaultSubobject<USprintActorComponent>(TEXT("SprintActorComponent"));
	LockedActorComponent = CreateDefaultSubobject<ULockedActorComponent>(TEXT("LockedActorComponent"));
	ParryActorComponent = CreateDefaultSubobject<UParryActorComponent>(TEXT("ParryActorComponent"));

	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	Tags.Add(FName("Player"));
}

void AACTCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//初始化组件并将人物信息传递给组件

	if (IsValid(BasicActionComponent))
	{
		BasicActionComponent->ACTCharacter = this;

	}
	if (IsValid(MeleeActionComponent))
	{
		MeleeActionComponent->ACTCharacter = this;
	}

	if (IsValid(DamageActorComponent))
	{
		DamageActorComponent->ACTCharacter = this;
	}

	if (IsValid(SprintActorComponent))
	{
		SprintActorComponent->ACTCharacter = this;
	}

	if (IsValid(LockedActorComponent))
	{
		LockedActorComponent->ACTCharacter = this;
	}

	if (IsValid(ParryActorComponent))
	{
		ParryActorComponent->ACTCharacter = this;
	}
}

void AACTCharacter::BeginPlay()
{
	Super::BeginPlay();
	ACTPlayerState = EPlayerState::EPS_Idle;
	//设置玩家控制器
	PlayerController = Cast<APlayerController>(GetController());
	//判断玩家控制器是否为空
	if (IsValid(PlayerController))
	{
		//设置增强输入本地玩家子系统
		EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		//判断增强输入本地玩家子系统是否为空
		if (EnhancedInputLocalPlayerSubsystem)
		{
			//每次添加映射先清除所有映射，避免重复添加
			EnhancedInputLocalPlayerSubsystem->ClearAllMappings();
			EnhancedInputLocalPlayerSubsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
	if (IsValid(GetMesh()))
	{
		GetMesh()->HideBoneByName("Weapon_r", PBO_None);
		GetMesh()->HideBoneByName("Weapon_l", PBO_None);
	}
	if (IsValid(DamageActorComponent))
	{
		DamageActorComponent->SaveThePoints();
	}

}

void AACTCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AACTCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	//设置增强输入组件
	EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	//判断增强输入组件是否为空
	if (IsValid(EnhancedInputComponent))
	{
		//Move
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AACTCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AACTCharacter::Move);
		//WalkRun
		EnhancedInputComponent->BindAction(WalkRunAction, ETriggerEvent::Triggered, this, &AACTCharacter::WalkRun);
		//Crouch
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AACTCharacter::Crouching);
		//Jump
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AACTCharacter::JumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AACTCharacter::JumpEnd);
		//Lock
		EnhancedInputComponent->BindAction(LockAction, ETriggerEvent::Triggered, this, &AACTCharacter::Lock);
		//Equip
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &AACTCharacter::Equiped);
		//Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AACTCharacter::Attack);
		//ChargeAttack
		EnhancedInputComponent->BindAction(ChargeAttackAction, ETriggerEvent::Triggered, this, &AACTCharacter::ChargeAttack);
		//ChargeAttack
		EnhancedInputComponent->BindAction(AvoidAction, ETriggerEvent::Triggered, this, &AACTCharacter::Avoid);
		//Locked
		EnhancedInputComponent->BindAction(LockedAction, ETriggerEvent::Triggered, this, &AACTCharacter::Locked);
		EnhancedInputComponent->BindAction(LockedLeftAction, ETriggerEvent::Triggered, this, &AACTCharacter::LeftLocked);
		EnhancedInputComponent->BindAction(LockedRightAction, ETriggerEvent::Triggered, this, &AACTCharacter::RightLocked);
		//Parry
		EnhancedInputComponent->BindAction(ParryAction, ETriggerEvent::Triggered, this, &AACTCharacter::ParryStart);
		EnhancedInputComponent->BindAction(ParryAction, ETriggerEvent::Completed, this, &AACTCharacter::ParryEnd);
	}
}

void AACTCharacter::ChangeState(EPlayerState IACTPlayerState)
{
	ACTPlayerState = IACTPlayerState;
}

void AACTCharacter::CanCombo()
{
	if (MoveAxis == FVector2D(0, 0))
	{
		ACTPlayerState = EPlayerState::EPS_Idle;
	}
	else
	{
		ACTPlayerState = EPlayerState::EPS_Move;
	}
}

void AACTCharacter::ResetCombo()
{
	if (MoveAxis == FVector2D(0, 0))
	{
		ACTPlayerState = EPlayerState::EPS_Idle;
		MeleeActionComponent->AttackCount = -1;
	}
	else
	{
		ACTPlayerState = EPlayerState::EPS_Idle;
		MeleeActionComponent->AttackCount = -1;
	}
}

void AACTCharacter::Move(const FInputActionValue& InputValue)
{
	//获取Key值且设置为向量2D
	FVector2D MovementVector = InputValue.Get<FVector2D>();
	//判断玩家控制器是否为空
	if (IsValid(PlayerController))
	{
		//将玩家输入的轴值传给MoveAxis
		MoveAxis = FVector2D(0, 0);
		MoveAxis = FVector2D(MovementVector.X, MovementVector.Y);
	}
}

void AACTCharacter::WalkRun(const FInputActionValue& InputValue)
{
	if (IsValid(BasicActionComponent))
	{
		BasicActionComponent->MoveChangeState();
	}
}

void AACTCharacter::Crouching(const FInputActionValue& InputValue)
{
	if (IsValid(BasicActionComponent))
	{
		BasicActionComponent->Crouching();
	}
}

void AACTCharacter::JumpStart(const FInputActionValue& InputValue)
{
	if (IsValid(BasicActionComponent))
	{
		BasicActionComponent->JumpStart();
	}
}

void AACTCharacter::JumpEnd(const FInputActionValue& InputValue)
{
	if (IsValid(BasicActionComponent))
	{
		BasicActionComponent->JumpEnd();
	}
}

void AACTCharacter::Lock(const FInputActionValue& InputValue)
{
	//获取Key值且设置为向量2D
	FVector2D LockVector = InputValue.Get<FVector2D>();
	if (IsValid(PlayerController))
	{
		if (!LockedActorComponent->bIsLocked)
		{
			AddControllerYawInput(LockVector.X * -1);
			AddControllerPitchInput(LockVector.Y);
		}
	}
}

void AACTCharacter::Equiped(const FInputActionValue& InputValue)
{
	if (IsValid(BasicActionComponent))
	{
		BasicActionComponent->Equiped();
	}
}

void AACTCharacter::Attack(const FInputActionValue& InputValue)
{
	if (IsValid(MeleeActionComponent))
	{
		MeleeActionComponent->Attack(); 
	}
}

void AACTCharacter::ChargeAttack(const FInputActionValue& InputValue)
{
	if (IsValid(MeleeActionComponent))
	{
		MeleeActionComponent->ChargeAttack();
	}
}

void AACTCharacter::Avoid(const FInputActionValue& InputValue)
{
	if (IsValid(SprintActorComponent))
	{
		SprintActorComponent->Sprint();
	}
}

void AACTCharacter::EnableTrace()
{
	if (IsValid(DamageActorComponent))
	{
		DamageActorComponent->EnableTrace();
	}

}

void AACTCharacter::DisableTrace()
{
	if (IsValid(DamageActorComponent))
	{
		DamageActorComponent->DisableTrace();
	}
}

void AACTCharacter::Locked(const FInputActionValue& InputValue)
{
	if (IsValid(LockedActorComponent))
	{
		LockedActorComponent->InputLock();
	}
}

void AACTCharacter::LeftLocked()
{
	if (IsValid(LockedActorComponent))
	{
		LockedActorComponent->LockSwitch(ELockedType::ELP_LeftLock);
	}
}

void AACTCharacter::RightLocked()
{
	if (IsValid(LockedActorComponent))
	{
		LockedActorComponent->LockSwitch(ELockedType::ELP_RightLock);
	}
}

void AACTCharacter::ParryStart(const FInputActionValue& InputValue)
{
	if (IsValid(ParryActorComponent))
	{
		ParryActorComponent->ParryStart();
	}
}

void AACTCharacter::ParryEnd(const FInputActionValue& InputValue)
{
	if (IsValid(ParryActorComponent))
	{
		ParryActorComponent->ParryEnd();
	}
}
