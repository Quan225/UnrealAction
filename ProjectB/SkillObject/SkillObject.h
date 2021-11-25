#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillObject.generated.h"

UCLASS()
class PROJECTB_API USkillObject : public UObject
{
	GENERATED_BODY()
	
public:
	USkillObject();
	UPROPERTY()
	FString SkillName;

public:
	virtual void ExecuteSkill();
	virtual void ExecuteSkill(class AGameCharacter* Caster);

	// 노티파이에서 호출하는 이벤트
	virtual void CustomHitEvent(class AGameCharacter* Caster);
	virtual void CustomSpawnEvent(class AGameCharacter* Caster);

protected:
	/* 설정된 위치에 구 형태의 충돌체를 발생시켜 충돌한 적을 반환한다. */
	TArray<class AEnemy*> CheckTargetBySphere(UWorld* World, AActor* Caster, FVector SphereCenter, float SphereRadius, bool bDrawDebugObj = false);
};
