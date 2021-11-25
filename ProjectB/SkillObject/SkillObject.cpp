#include "SkillObject.h"
#include <ProjectB/Enemy.h>
#include <DrawDebugHelpers.h>

USkillObject::USkillObject()
{

}

void USkillObject::ExecuteSkill()
{
	UE_LOG(LogTemp, Log, TEXT("Execute Base Skill"));
}

void USkillObject::ExecuteSkill(AGameCharacter* Caster)
{
	UE_LOG(LogTemp, Log, TEXT("Execute Base Skill2"));
}

void USkillObject::CustomHitEvent(AGameCharacter* Caster)
{
	UE_LOG(LogTemp, Log, TEXT("Custom Skill Event Not Set"));
}

void USkillObject::CustomSpawnEvent(AGameCharacter* Caster)
{
	UE_LOG(LogTemp, Log, TEXT("Custom Skill Event Not Set"));
}


TArray<AEnemy*> USkillObject::CheckTargetBySphere(UWorld* World, AActor* Caster, FVector SphereCenter, float SphereRadius, bool bDrawDebugObj)
{
	TArray<AEnemy*> CollEnemys;
	TArray<FOverlapResult> overlapResults;

	FCollisionQueryParams collParam(NAME_None, false);
	collParam.bFindInitialOverlaps = true;

	bool isOverlap = World->OverlapMultiByChannel(
		overlapResults,
		SphereCenter,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(SphereRadius),
		collParam
	);

	if (isOverlap)
	{
		for (const auto& result : overlapResults)
		{
			AEnemy* Target = Cast<AEnemy>(result.GetActor());
			if (Target == nullptr)
				continue;
			CollEnemys.Add(Target);
		}
	}

	if (bDrawDebugObj)
	{
		DrawDebugSphere(World, SphereCenter, SphereRadius, 24, FColor::Green, false, 1.0f);
	}
	
	return CollEnemys;
}

