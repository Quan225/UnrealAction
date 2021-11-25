#include "PlayerCharacter.h"
#include "Inventory/Item.h"
#include "SkillObject/SkillObject.h"
#include "SkillObject/DummySkill.h"
#include "Inventory/Item.h"

#include "SkillObject/Projectile.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = GetCapsuleComponent();
	GetCapsuleComponent()->SetCollisionProfileName("Player");
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> cMesh(TEXT("SkeletalMesh'/Game/Character/Meshes/Wraith.Wraith'"));
	if (cMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(cMesh.Object);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Load Faild : Player Character Mesh"));
	}

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	GetMesh()->HideBoneByName("weapon_r", EPhysBodyOp::PBO_None);

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> animInst(TEXT("AnimBlueprint'/Game/BluePrint/BP_PlayerAnim.BP_PlayerAnim_C'"));
	if (animInst.Class != nullptr)
	{
		GetMesh()->SetAnimInstanceClass(animInst.Class);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Load Faild : Player Character Blueprint"));
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeLocation(FVector(0, 0, 80.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 5.0f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->ProbeChannel = ECC_GameTraceChannel6;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	DataComponent = CreateDefaultSubobject<UPlayerDataComponent>(TEXT("DataComponent"));	
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	SkillComponent = CreateDefaultSubobject<UPlayerCharacterSkillComponent>(TEXT("SkillComponent"));
	
	CharacterMesh = GetMesh();
	State = ECharacterState::Idle;

	bReplicates = true;
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AnimInst = Cast<UPlayerAnimInst>(GetMesh()->GetAnimInstance());
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	DataComponent->Initialize("Player_Wraith");

	FName weaponSocketName = TEXT("Weapon_Right");
	//FRotator rotator;
	//FVector location;
	Weapon = GetWorld()->SpawnActor<AWeaponActor>(FVector::ZeroVector, FRotator::ZeroRotator);
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), weaponSocketName);
	Weapon->Equit(this);

	InitStatus("Player_Wraith");
	InventoryComponent->SetOwningCharacter(this);
	if (!InventoryComponent->CreateInventoryWidget(GetInventoryCapacity()))
	{
		UE_LOG(LogTemp, Log, TEXT("Create Inventory Is Failed"));
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::EndJump);
	PlayerInputComponent->BindAction("AttackLight", IE_Pressed, this, &APlayerCharacter::AttackRight);
	PlayerInputComponent->BindAction("AttackHeavy", IE_Pressed, this, &APlayerCharacter::TestMouseR);
	PlayerInputComponent->BindAction("InventoryToggle", IE_Pressed, this, &APlayerCharacter::ToggleInventory);
	PlayerInputComponent->BindAction("Skill_Q", IE_Pressed, this, &APlayerCharacter::PresseSkillQ);
	PlayerInputComponent->BindAction("Skill_Q", IE_Released, this, &APlayerCharacter::ReleaseSkillQ);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("MouseTurnX", this, &APlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("MouseTurnY", this, &APlayerCharacter::AddControllerPitchInput);
}

void APlayerCharacter::ToggleInventory()
{
	if (!bIsInventoryOpen)
		InventoryComponent->OpenInventory();
	else
		InventoryComponent->CloseInventory();
	
	bIsInventoryOpen = !bIsInventoryOpen;

	Cast<APlayerController>(GetController())->bShowMouseCursor = bIsInventoryOpen;
	// Cast<APlayerController>(GetController())->bEnableClickEvents = bIsInventoryOpen;
	// Cast<APlayerController>(GetController())->bEnableMouseOverEvents = bIsInventoryOpen;
}

void APlayerCharacter::Server_UseSkill_Implementation(EAxisInputType AxisType, EButtonInputType ButtonType)
{
	if (!CanAttack())
		return;

	if (SkillComponent->UseSkill(AxisType, ButtonType))
	{
		Multi_UseSkill(AxisType, ButtonType);
	}
}

void APlayerCharacter::Multi_UseSkill_Implementation(EAxisInputType AxisType, EButtonInputType ButtonType)
{
	if (GetLocalRole() == ROLE_Authority)
		return;

	SkillComponent->UseSkill(AxisType, ButtonType);
}

void APlayerCharacter::UseSkill(EAxisInputType AxisType, EButtonInputType ButtonType)
{
	if (!CanAttack())
	{
		UE_LOG(LogTemp, Log, TEXT("CanAttack False"))
		return;
	}

	if (HasAuthority())
	{
		SkillComponent->UseSkill(AxisType, ButtonType);
	}
	else
	{
		Server_UseSkill(ReadAxisInputState(), CurButtonState);
	}
}

void APlayerCharacter::AttackRight()
{
	if (!CanAttack())
		return;

	Server_UseSkill(ReadAxisInputState(), CurButtonState);
}

void APlayerCharacter::ServerTestMouseR_Implementation()
{
}

void APlayerCharacter::Multi_TestMouseR_Implementation()
{
}

void APlayerCharacter::TestMouseR()
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerTestMouseR();
	}
}

void APlayerCharacter::PresseSkillQ()
{
	CurButtonState = EButtonInputType::Skill_Q;
	UseSkill(ReadAxisInputState(), CurButtonState);
}

void APlayerCharacter::ReleaseSkillQ()
{
	if (CurButtonState == EButtonInputType::Skill_Q)
		CurButtonState = EButtonInputType::None;
}

void APlayerCharacter::InitStatus(FString InitCharacterName)
{
	DataComponent->GetCharacterStat(MaxHp, MaxStemina);
	CurHp = MaxHp;
	CurStemina = MaxStemina;

	InventoryCapacity = 24;
}

void APlayerCharacter::OnRep_CurHp()
{
	Super::OnRep_CurHp();
}

void APlayerCharacter::OnRep_IsStun()
{
	Super::OnRep_IsStun();

	if (bIsStun)
	{
		Stun();
	}
}

void APlayerCharacter::OnRep_IsDead()
{
	Super::OnRep_IsDead();

	if (bIsDead)
	{
		Dead();
	}
}

bool APlayerCharacter::SetState(ECharacterState NewState)
{
	switch (NewState)
	{
	case ECharacterState::Idle:
		ResetAttackFlags();
		SetIsStun(false);
		AnimInst->SetIsStun(bIsStun);
		State = NewState;
		bIsInvisible = false;
		return true;

	case ECharacterState::Attack:
		if (CanAttack())
		{
			SetIsAttack(true);
			AnimInst->SetIsAttack(bIsAttack);
			State = NewState;
			return true;
		}
		return false;

	case ECharacterState::Stun:
		if (Stun())
		{
			State = NewState;
			return true;
		}
		return false;

	case ECharacterState::Dodge:
		if (State == ECharacterState::Idle || State == ECharacterState::Attack)
		{
			Dodge(ReadAxisInputState()); // Todo :이러면 판정 꼬임 이럼안됨
			State = NewState;
		}
		return true;
		
	case ECharacterState::Dead:
		Dead();
		State = NewState;
		return true;

	default:
		return false;
	}
}

void APlayerCharacter::Dead()
{
	AnimInst->StopAllMontage();
	SetIsDead(true);
	AnimInst->SetIsDead(bIsDead);
	SetCurHp(0.0f);
}

bool APlayerCharacter::Stun()
{
	Super::Stun();

	if (State == ECharacterState::Attack)
	{
		ResetAttackFlags();
	}
	SetIsStun(true);
	AnimInst->SetIsStun(bIsStun);

	// StandAlone || Client
	if (IsLocallyControlled() || GetLocalRole() < ROLE_Authority)
	{
		USoundCue* PlaySound = DataComponent->FindCharacterSound(ECharacterSoundType::Pain);
		if (PlaySound != nullptr)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), PlaySound, GetActorLocation());
	}

	// Montage길이 확인하도록 변경 필요
	FTimerHandle HitCheckEndTimer;

	GetWorld()->GetTimerManager().SetTimer(HitCheckEndTimer, FTimerDelegate::CreateLambda([&]()
		{
			SetState(ECharacterState::Idle);
		}), 0.5f, false);

	return true;
}

bool APlayerCharacter::CanAttack()
{
	if (State == ECharacterState::Dead || State == ECharacterState::Stun || State == ECharacterState::Dodge)
	{
		return false;
	}

	return true;
}

EAxisInputType APlayerCharacter::ReadAxisInputState()
{
	float VerKeyState = GetInputAxisValue("MoveForward");
	float HorKeyState = GetInputAxisValue("MoveRight");

	if (VerKeyState == 0.0f && HorKeyState == 0.0f)
		return EAxisInputType::None;

	FVector AxisVector = FVector(VerKeyState, HorKeyState, 0.0f); // 키보드 입력 벡터
	FVector CamVector = Camera->GetForwardVector(); // 카메라 forawrd 벡터
	CamVector.Z = 0;
	CamVector.Normalize();

	FVector RotAxisVector = CamVector.Rotation().RotateVector(AxisVector); // 키보드 벡터를 카메라 방향에 맞춰 회전
	AxisVector = RotAxisVector;

	FVector ActorForward = GetActorForwardVector(); // 액터의 forward 벡터
	ActorForward.Z = 0.0f;
	
	AxisVector.Normalize();
	ActorForward.Normalize();

	// (Front : 0) ~ (Back : Pi)
	float AngleDiff = FVector::DotProduct(AxisVector, ActorForward);
	// (Right : 0 ~ 1) ~ (Left : -1 ~ 0)
	float CheckRight = FVector::DotProduct(FVector::UpVector, (FVector::CrossProduct(ActorForward, AxisVector)));

	//UE_LOG(LogTemp, Log, TEXT("Key : %f, %f"), VerKeyState, HorKeyState);
	//UE_LOG(LogTemp, Log, TEXT("AxisVector : %f, %f, %f"), AxisVector.X, AxisVector.Y, AxisVector.Z);
	//UE_LOG(LogTemp, Log, TEXT("AngleDiff : %f"), AngleDiff); // 현재 입력과 액터 정면의 각(Radian)
	//UE_LOG(LogTemp, Log, TEXT("CheckRight : %f"), CheckRight); // 업 벡터와 (입력, 액터 정면)의 외적의 각

	if (AngleDiff >= 0.5f)
	{
		//UE_LOG(LogTemp, Log, TEXT("Forward"));
		return EAxisInputType::Front;
	}
	else if (AngleDiff >= -0.5f)
	{
		if (CheckRight >= 0)
		{
			//UE_LOG(LogTemp, Log, TEXT("Right"));
			return EAxisInputType::Right;
		}
		else
		{
			//UE_LOG(LogTemp, Log, TEXT("Left"));
			return EAxisInputType::Left;
		}
	}
	else
	{
		//UE_LOG(LogTemp, Log, TEXT("Back"));
		return EAxisInputType::Back;
	}
}

void APlayerCharacter::ResetAttackFlags()
{
	AnimInst->StopAllMontage();
	SetIsAttack(false);
	AnimInst->SetIsAttack(false);
	SkillComponent->ResetChainFlags();
}

//void APlayerCharacter::StartJump_Implementation()
//{
//
//	UGameplayStatics::ApplyDamage(this, 10.0f, nullptr, this, UDamageType::StaticClass());
//}

void APlayerCharacter::StartJump()
{

	CurButtonState = EButtonInputType::Space;
	Server_UseSkill(ReadAxisInputState(), CurButtonState);



//	UGameplayStatics::ApplyDamage(this, 10.0f, nullptr, this, UDamageType::StaticClass());

	/*AProjectile* SpawnProjectile = GetWorld()->SpawnActor<AProjectile>(GetActorLocation(), FRotator::ZeroRotator);
	SpawnProjectile->InitProjectile(this, 1400.0f);*/

	//FVector MoveDir = GetActorForwardVector();
	//MoveDir.Z = 0.0f;
	//MoveDir.Normalize();

	//LaunchCharacter((MoveDir * 1000.0f) + FVector(0.0f, 0.0f, 100.0f), false, false);


	//AnimInst->SetIsAttack(true);
	//bPressedJump = true;
}

void APlayerCharacter::EndJump()
{
	if(CurButtonState == EButtonInputType::Space)
		CurButtonState = EButtonInputType::None;

	//AnimInst->SetIsAttack(false);
	//bPressedJump = false;
}

void APlayerCharacter::MoveForward(float AxisValue)
{
	if (State != ECharacterState::Idle)
	{
		return;
	}

	if ((Controller != nullptr) && (AxisValue != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, AxisValue);
	}
}

void APlayerCharacter::MoveRight(float AxisValue)
{
	if (State != ECharacterState::Idle)
	{
		return;
	}

	if ((Controller != nullptr) && (AxisValue != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, AxisValue);
	}
}

void APlayerCharacter::AddControllerPitchInput(float Val)
{
	if (bIsInventoryOpen)
		return;

	Super::AddControllerPitchInput(Val);
}

void APlayerCharacter::AddControllerYawInput(float Val)
{
	if (bIsInventoryOpen)
		return;

	Super::AddControllerYawInput(Val);
}

void APlayerCharacter::RotateActorToFront(bool bIsReadKey)
{
	// 스프링암이 바라보는 방향으로 액터를 회전
	FRotator ActorRot = GetActorRotation();
	ActorRot.Yaw = SpringArm->GetTargetRotation().Yaw;

	if (bIsReadKey)
	{
		float VerKeyState = GetInputAxisValue("MoveForward");
		float HorKeyState = GetInputAxisValue("MoveRight");

		FRotator KeyStateRot = FVector(VerKeyState, HorKeyState, 0.0f).Rotation();

		ActorRot.Yaw += KeyStateRot.Yaw;
	}

	SetActorRotation(ActorRot);
}

// WIP : 회피동작
bool APlayerCharacter::Dodge(EAxisInputType AxisType)
{
	AnimInst->SetIsDodge(true);
	bIsInvisible = true;
	ResetAttackFlags();

	FTimerHandle DodgeEndTimer2;

	GetWorld()->GetTimerManager().SetTimer(DodgeEndTimer2, FTimerDelegate::CreateLambda([&]()
		{
			SetState(ECharacterState::Idle);
			DirDodge = FVector::ZeroVector;
			bIsInvisible = false;
			AnimInst->SetIsDodge(false);

		}), 0.5f, false);

	return true;

	float VerKeyState = GetInputAxisValue("MoveForward");
	float HorKeyState = GetInputAxisValue("MoveRight");
	FVector AxisVector = FVector(VerKeyState, HorKeyState, 0.0f); // 키보드 입력 벡터

	FVector CamVector = Camera->GetForwardVector(); // 카메라 forawrd 벡터
	CamVector.Z = 0;
	CamVector.Normalize();

	AxisVector = CamVector.Rotation().RotateVector(AxisVector);
	AxisVector.Z = 0.0f;
	AxisVector.Normalize();
	
	float AngleDiff = FVector::DotProduct(CamVector, AxisVector);
	float CheckRight = FVector::DotProduct(FVector::UpVector, (FVector::CrossProduct(CamVector, AxisVector)));

//	if (CheckRight < 0)
//		AngleDiff = -AngleDiff;

	UE_LOG(LogTemp, Log, TEXT("%f, %f"), AngleDiff, CheckRight);

	AnimInst->SetDodgeDegree(AngleDiff);
	DirDodge = AxisVector;



	FTimerHandle DodgeEndTimer;

	GetWorld()->GetTimerManager().SetTimer(DodgeEndTimer, FTimerDelegate::CreateLambda([&]()
		{
			SetState(ECharacterState::Idle);
			DirDodge = FVector::ZeroVector;
			bIsInvisible = false;
			AnimInst->SetIsDodge(false);
			AnimInst->SetDodgeDegree(0.0f);


			bUseControllerRotationYaw = false;
			GetCharacterMovement()->bOrientRotationToMovement = true;
		}), 0.5f, false);


	return false;
}

void APlayerCharacter::Client_PickUpItem_Implementation(int32 PickItemData, int32 PickAddCount)
{
	UE_LOG(LogTemp, Log, TEXT("PC => Idx : %d, Count : %d"), PickItemData, PickAddCount);
	InventoryComponent->AddItem(PickItemData, PickAddCount);
}

float APlayerCharacter::TakeDamage(float DamageAMount, FDamageEvent const& DamageEnvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (GetLocalRole() < ROLE_Authority)
		return -1.0f;

	if (bIsInvisible)
		return 0.0f;

	float damageApplied = CurHp - DamageAMount;
	SetCurHp(damageApplied);
	
	if (CurHp <= 0.0f)
	{
		SetState(ECharacterState::Dead);
		return -1.0f;
	}

	SetState(ECharacterState::Stun);

	return damageApplied;
}
