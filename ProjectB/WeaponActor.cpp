#include "WeaponActor.h"
#include <Kismet/KismetMathLibrary.h>
#include "PlayerCharacter.h"

AWeaponActor::AWeaponActor()
{
	PrimaryActorTick.bCanEverTick = true;
	

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> wMesh(TEXT("SkeletalMesh'/Game/Blade_HeroSword11/SK_Blade_HeroSword11.SK_Blade_HeroSword11'"));
	if (wMesh.Succeeded())
	{
		WeaponMesh->SetSkeletalMesh(wMesh.Object);
	}

	WeaponPower = 30.0f;
}

void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsEnableTrace)
	{
		TraceWeaponRange();
	}
}

void AWeaponActor::EnableCollider()
{
	IsEnableTrace = true;
	if (IgnoreDamageActor.Num() != 0)
	{
		IgnoreDamageActor.Empty();
	}
}

void AWeaponActor::DisableCollider()
{
	IsEnableTrace = false;
	IgnoreDamageActor.Empty();
}

void AWeaponActor::Equit(AActor* EquitCharacter)
{
	if (EquitCharacter != nullptr)
	{
		SetOwner(EquitCharacter);
	}
}

void AWeaponActor::TraceWeaponRange()
{
	TArray<FHitResult> outHit;

	FVector start = WeaponMesh->GetSocketLocation("Collider_Start");
	FVector end = WeaponMesh->GetSocketLocation("Collider_End");

	FCollisionQueryParams collParams;
	collParams.bFindInitialOverlaps = true;

	float spRadios = 30.0f; // Collider두께 하드코딩

	// 범위 보고싶다면 true
	if (false)
	{
		DrawDebugLine(GetWorld(), start, end, FColor::Green, false, -1, 0, 1);
		DrawDebugSphere(GetWorld(), start, FCollisionShape::MakeSphere(spRadios).GetSphereRadius(), 20, FColor::Green, false, 1.0f);
		DrawDebugSphere(GetWorld(), end, FCollisionShape::MakeSphere(spRadios).GetSphereRadius(), 20, FColor::Green, false, 1.0f);
	}
	
	GetWorld()->SweepMultiByChannel(outHit, start, end, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeSphere(spRadios), collParams);

	if (outHit.Num() > 0)
	{
		for (int i = 0; i < outHit.Num(); i++)
		{
			if (outHit[i].GetActor() != nullptr)
			{
				if(IgnoreDamageActor.Find(outHit[i].GetActor()) == INDEX_NONE)
				{
					UE_LOG(LogTemp, Log, TEXT("Hit Enemy : %s"), *outHit[i].GetActor()->GetName());
					AGameCharacter* temp = Cast<AGameCharacter>(outHit[i].GetActor());
					if (temp == nullptr)
						continue;
					
					temp->TakeKnockBack(GetOwner()->GetActorLocation(), 800.0f);

					APlayerCharacter* pc = Cast<APlayerCharacter>(GetOwner());
					if (pc != nullptr)
					{
						FTransform SpawnTr;
						SpawnTr.SetLocation(outHit[i].Location);
						SpawnTr.SetScale3D(FVector(0.3f, 0.3f, 0.3f));
						SpawnTr.SetRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), outHit[i].Location).Quaternion());
						pc->SpawnImpactEffect(pc->DataComponent->FindImpactEffect("BaseCombo1"), SpawnTr);
						pc->PlaySoundCue(pc->DataComponent->FindImpactSound("BaseCombo1"), SpawnTr.GetLocation());


						// Enemy에 있던것도 하나로 뭉쳐주세요
						UAnimMontage* CurMontage = pc->AnimInst->GetCurrentActiveMontage();
						if (CurMontage != nullptr)
						{
							int32 CurPlayRate = pc->AnimInst->Montage_GetPlayRate(CurMontage);
							pc->AnimInst->Montage_SetPlayRate(CurMontage, 0.03f);

							FTimerHandle HitStopTimer;

							GetWorld()->GetTimerManager().SetTimer(HitStopTimer, FTimerDelegate::CreateLambda([CurMontage, CurPlayRate, pc]()
								{
									if (CurMontage == nullptr)
									{
										UE_LOG(LogTemp, Log, TEXT("CurMontage Is Null"));
										return;
									}
									if (CurMontage != pc->AnimInst->GetCurrentActiveMontage())
										return;

									pc->AnimInst->Montage_SetPlayRate(CurMontage, CurPlayRate);
								}), 0.13f, false);
						}
					}

					UGameplayStatics::ApplyDamage(outHit[i].GetActor(), WeaponPower, nullptr, this, UDamageType::StaticClass());
					IgnoreDamageActor.Add(outHit[i].GetActor());
				}
			}
		}
	}
}

