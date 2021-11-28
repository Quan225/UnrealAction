#include "Enemy.h"
#include "EnemySpawnerDirector.h"
#include "Inventory/ItemActor.h"
#include "SkillData.h"
#include "ProjectBGameInstance.h"
#include "ProjectBGameModeBase.h"
#include "ProjectBGameStateBase.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetRelativeLocation(FVector(0, 0, 80.0f));
	GetCapsuleComponent()->SetCapsuleSize(50.0f, 70.0f);
	GetCapsuleComponent()->SetCollisionProfileName("Enemy");
	GetCapsuleComponent()->SetEnableGravity(true);
	RootComponent = GetCapsuleComponent();

	GetMesh()->SetupAttachment(RootComponent);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -70.0f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;

	RootComponent->SetWorldScale3D(FVector(1.3f, 1.3f, 1.3f));

	DataComponent = CreateDefaultSubobject<UEnemyDataComponent>(TEXT("DataComponent"));

	SkillComponent = CreateDefaultSubobject<UEnemySkillComponent>(TEXT("SkillComponent"));

	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;


	ConstructorHelpers::FClassFinder<UAnimInstance> GunClass(TEXT("AnimBlueprint'/Game/BluePrint/ABP_Dusk_Gun.ABP_Dusk_Gun_C'"));
	if (GunClass.Class != nullptr)
	{
		GunAnimClass = GunClass.Class;
	}
	ConstructorHelpers::FClassFinder<UAnimInstance> SwordClass(TEXT("AnimBlueprint'/Game/BluePrint/BP_EnemyAnim.BP_EnemyAnim_C'"));
	if (SwordClass.Class != nullptr)
	{
		SwordAnimClass = SwordClass.Class;
	}

	MatAppearance = 0.0f;
	IsAppearance = true;
	bReplicates = true;
}

void AEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateMaterialAppearance(DeltaTime);

	if (IsWeaponColliderEnable && CurAttackType != EEnemyWeaponRangeType::None)
	{
		TraceWeaponRange(CurAttackType);
	}
}

float AEnemy::TakeDamage(float DamageAMount, FDamageEvent const& DamageEnvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (GetLocalRole() < ROLE_Authority)
		return -1.0f;

	if (CurHp <= 0.0f)
		return -1.0f;

	float damageApplied = CurHp - DamageAMount;
	SetCurHp(damageApplied);

	if (CurHp <= 0.0f)
	{
		SetState(ECharacterState::Dead);
		return -1.0f;
	}

	SetState(ECharacterState::Stun);

	return DamageAMount;
}

void AEnemy::Dead()
{
	CurHp = 0.0f;
	IsAppearance = false;
	AnimInst->StopAllMontage();
	SetIsDead(true);
	AnimInst->SetIsDead(bIsDead);
	
	AEnemyAIController* aiCon = Cast<AEnemyAIController>(GetController());
	if (aiCon != nullptr)
		aiCon->StopAI();

	FTimerHandle DeathDelayTimer;

	GetWorld()->GetTimerManager().SetTimer(DeathDelayTimer, FTimerDelegate::CreateLambda([&]()
		{
			AProjectBGameStateBase* gs = Cast<AProjectBGameStateBase>(GetWorld()->GetGameState());
			if (gs != nullptr)
			{
				//gs->KillCount++;
			}
			else
			{
				return;
			}

			if (GetLocalRole() == ROLE_Authority)
			{
				AProjectBGameModeBase* gm = Cast<AProjectBGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
				if (gm != nullptr)
				{
					gm->SpawnLevelItem(GetActorLocation());
					gm->KillEnemy(1);
				}
			}
			
			Destroy();
			
		}), 2.0f, false);
}

void AEnemy::Multi_InitCharacter_Implementation(const FString& InitCharacterName)
{
	UProjectBGameInstance* GameInst = Cast<UProjectBGameInstance>(GetGameInstance());
	if (GameInst == nullptr)
		return;

	DataComponent->Initialize(InitCharacterName);
	
	FEnemyInfo ChInfo = GameInst->GetEnemyInfo(InitCharacterName);

	GetMesh()->SetSkeletalMesh(ChInfo.SkeletalMesh);
	
	//UClass* AnimationClass = LoadObject<UClass>(NULL, *ChInfo.AnimInstRoute);
	//GetMesh()->SetAnimInstanceClass(AnimationClass);


	if (InitCharacterName == "Dusk_Sword")
	{
		GetMesh()->SetAnimInstanceClass(SwordAnimClass);
	}
	else
	{
		GetMesh()->SetAnimInstanceClass(GunAnimClass);
	}

	AnimInst = Cast<UCharacterAnimInst>(GetMesh()->GetAnimInstance());

	if (GetLocalRole() == ROLE_Authority)
	{
		CharacterName = DataComponent->GetCharacterName();
		InitStatus(DataComponent->GetCharacterName());
	}
}

void AEnemy::InitStatus(FString InitCharacterName)
{
	DataComponent->GetCharacterStat(MaxHp, MaxStamina);
	CurHp = MaxHp;
	CurStamina = MaxStamina;
}

bool AEnemy::SetState(ECharacterState NewState)
{
	switch (NewState)
	{
	case ECharacterState::Idle:
		AnimInst->SetIsAttack(false);
		ResetAttackFlag();
		State = NewState;
		return true;

	case ECharacterState::Attack:
		SetIsAttack(true);
		AnimInst->SetIsAttack(bIsAttack);
		State = NewState;
		return true;

	case ECharacterState::Stun:
		Stun();

		State = NewState;
		return true;

	case ECharacterState::Dead:
		Dead();
		State = NewState;
		return true;

	default:
		return false;
	}
}

bool AEnemy::CanAttack()
{
	if (State == ECharacterState::Attack)
	{
		return false;
	}

	if (State == ECharacterState::Dead || State == ECharacterState::Stun)
	{
		return false;
	}

	return true;
}

void AEnemy::ResetAttackFlag()
{
	AnimInst->StopAllMontage();
	SetIsAttack(false);
	AnimInst->SetIsAttack(bIsAttack);
	SkillComponent->ResetChainFlags();

	SetIsWeaponColliderEnable(false);
	SetCurAttackType(EEnemyWeaponRangeType::None);
}

void AEnemy::TraceWeaponRange(EEnemyWeaponRangeType RangeType)
{
	switch (RangeType)
	{
	case EEnemyWeaponRangeType::LeftSword:
		TraceWeaponRange(GetMesh()->GetSocketLocation("Weapon_Left_Start"), GetMesh()->GetSocketLocation("Weapon_Left_End"));
		break;
	case EEnemyWeaponRangeType::RightSword:
		TraceWeaponRange(GetMesh()->GetSocketLocation("Weapon_Right_Start"), GetMesh()->GetSocketLocation("Weapon_Right_End"));
		break;
	case EEnemyWeaponRangeType::TwinSword:
		TraceWeaponRange(GetMesh()->GetSocketLocation("Weapon_Left_Start"), GetMesh()->GetSocketLocation("Weapon_Left_End"));
		break;
	default:
		break;
	}	
}

void AEnemy::TraceWeaponRange(FVector Start, FVector End)
{
	TArray<FHitResult> outHit;
	FCollisionQueryParams collParams;
	collParams.bFindInitialOverlaps = true;

	float spRadios = 30.0f; // Collider두께 하드코딩

	// 범위 보고싶다면 true
	if (false)
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, -1, 0, 1);
		DrawDebugSphere(GetWorld(), Start, FCollisionShape::MakeSphere(spRadios).GetSphereRadius(), 20, FColor::Green, false, 1.0f);
		DrawDebugSphere(GetWorld(), End, FCollisionShape::MakeSphere(spRadios).GetSphereRadius(), 20, FColor::Green, false, 1.0f);
	}

	GetWorld()->SweepMultiByChannel(outHit, Start, End, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel4, FCollisionShape::MakeSphere(spRadios), collParams);

	if (outHit.Num() > 0)
	{
		for (int i = 0; i < outHit.Num(); i++)
		{
			if (outHit[i].GetActor() != nullptr)
			{
				if (IgnoreDamageActor.Find(outHit[i].GetActor()) == INDEX_NONE)
				{
					AGameCharacter* temp = Cast<AGameCharacter>(outHit[i].GetActor());
					if (temp == nullptr)
						continue;

					temp->TakeKnockBack(GetActorLocation(), 800.0f);
					UGameplayStatics::ApplyDamage(outHit[i].GetActor(), 18.0f, nullptr, this, UDamageType::StaticClass());

					FTransform SpawnTr;
					SpawnTr.SetLocation(outHit[i].Location);
					SpawnImpactEffect(DataComponent->FindImpactEffect("Swing"), SpawnTr);
					PlaySoundCue(DataComponent->FindImpactSound("Swing"), SpawnTr.GetLocation());

					UAnimMontage* CurMontage = AnimInst->GetCurrentActiveMontage();
					if (CurMontage != nullptr)
					{
						int32 CurPlayRate = AnimInst->Montage_GetPlayRate(CurMontage);
						AnimInst->Montage_SetPlayRate(CurMontage, 0.03f);

						FTimerHandle HitStopTimer;

						GetWorld()->GetTimerManager().SetTimer(HitStopTimer, FTimerDelegate::CreateLambda([CurMontage, CurPlayRate, this]()
							{
								if (CurMontage == nullptr)
								{
									UE_LOG(LogTemp, Log, TEXT("CurMontage Is Null"));
									return;
								}
								if (CurMontage != AnimInst->GetCurrentActiveMontage())
									return;

								AnimInst->Montage_SetPlayRate(CurMontage, CurPlayRate);
							}), 0.15f, false);
					}


					IgnoreDamageActor.Add(outHit[i].GetActor());
				}
			}
		}
	}
}

void AEnemy::SetIsWeaponColliderEnable(bool IsEnable)
{
	IsWeaponColliderEnable = IsEnable;
	if (IgnoreDamageActor.Num() != 0)
	{
		IgnoreDamageActor.Empty();
	}
}

void AEnemy::UpdateMaterialAppearance(float DeltaTime)
{
	if (IsAppearance)
	{
		if (MatAppearance >= 1.0f)
			return;
		MatAppearance += DeltaTime;
	}
	else
	{
		if (MatAppearance <= 0.0f)
			return;
		MatAppearance -= DeltaTime;
	}
	
	MatAppearance = FMath::Clamp(MatAppearance, 0.0f, 1.0f);
	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("Appearance"), MatAppearance);
}

void AEnemy::OnRep_IsStun()
{
	Super::OnRep_IsStun();

	UE_LOG(LogTemp, Log, TEXT("OnRep_IsStun"));
	if (bIsStun)
	{
		Stun();
	}
}

void AEnemy::OnRep_IsDead()
{
	Super::OnRep_IsDead();

	if (bIsDead)
	{
		Dead();
	}
}

void AEnemy::OnRep_CharacterName()
{
	Super::OnRep_CharacterName();

	if (GetLocalRole() < ROLE_Authority)
	{
		Multi_InitCharacter(CharacterName);
	}
}

bool AEnemy::Stun()
{
	Super::Stun();

	if (GetState() == ECharacterState::Attack)
	{
		ResetAttackFlag();
	}

	UE_LOG(LogTemp, Log, TEXT("Chain IsStun"));
	SetIsStun(true);
	AnimInst->SetIsStun(true);

	FTimerHandle HitCheckEndTimer;

	GetWorld()->GetTimerManager().SetTimer(HitCheckEndTimer, FTimerDelegate::CreateLambda([&]()
		{
			SetIsStun(false);
			AnimInst->SetIsStun(bIsStun);
			SetState(ECharacterState::Idle);
		}), 0.5f, false);

	return true;
}

void AEnemy::Multi_Attack_Implementation()
{
	if (GetLocalRole() == ROLE_Authority)
		return;

	SkillComponent->UseSkill(EAxisInputType::None, EButtonInputType::None);
}

void AEnemy::Attack()
{
	if (GetLocalRole() < ROLE_Authority)
		return;

	if (SkillComponent->UseSkill(EAxisInputType::None, EButtonInputType::None))
	{
		Multi_Attack();
	}
}
