#include "ItemActor.h"
#include <ProjectB/PlayerCharacter.h>
#include <Kismet/GameplayStatics.h>
#include <ProjectB/ProjectBGameModeBase.h>

AItemActor::AItemActor()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject <USkeletalMeshComponent>(TEXT("ItemMesh"));
	RootComponent = ItemMesh;

	OverlapCollider = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	OverlapCollider->SetCollisionProfileName(TEXT("OverlapAll"));
	OverlapCollider->OnComponentBeginOverlap.AddDynamic(this, &AItemActor::OnOverlapBegin);

	OverlapCollider->AttachTo(ItemMesh);
	bReplicates = true;
	//OverlapCollider->AttachToComponent(ItemMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FName(*OverlapCollider->GetName()));
}

void AItemActor::BeginPlay()
{
	Super::BeginPlay();
	if (ItemData == nullptr)
	{
		InitItemData(2);
	}
}

void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemActor::InitItemData(int32 NewItemIdx)
{
	ItemData = NewObject<UItem>(this, UItem::StaticClass());
	ItemData->SetItemIdx(NewItemIdx);
	ItemMesh->SetSkeletalMesh(ItemData->GetItemMesh(NewItemIdx));

	ItemIdx = ItemData->GetItemIdx();
	ItemCount = ItemData->GetItemCount();

	if (ItemCount <= 0)
	{
		ItemCount = 1;
		ItemData->SetItemCount(ItemCount);
	}
}

void AItemActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Server_PickUp(OtherActor);
}

bool AItemActor::Server_PickUp(AActor* OverlapActor)
{
	if (GetLocalRole() < ROLE_Authority)
		return false;

	APlayerCharacter* pc = Cast<APlayerCharacter>(OverlapActor);
	if (pc == nullptr)
		return false;

	if (!pc->InventoryComponent->CanAddItem())
		return false;

	if (!pc->InventoryComponent->AddItem(ItemData, ItemData->GetItemCount()))
	{
		UE_LOG(LogTemp, Log, TEXT("Here"));
		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("Try Client PickUp"));
	
	pc->Client_PickUpItem(ItemIdx, ItemCount);
	
	//Client_PickUp(OverlapActor);
	
	AProjectBGameModeBase* gm = Cast<AProjectBGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (gm != nullptr)
		gm->PickUpItem(this);

	return true;
}

void AItemActor::Client_PickUp_Implementation(AActor* OverlapActor)
{

	UE_LOG(LogTemp, Log, TEXT("Try Client PickUp 1"));
	if (GetLocalRole() == ROLE_Authority)
		return;


	UE_LOG(LogTemp, Log, TEXT("Try Client PickUp 2"));
	APlayerCharacter* pc = Cast<APlayerCharacter>(OverlapActor);
	if (pc == nullptr)
		return;

	UE_LOG(LogTemp, Log, TEXT("Idx : %d, Count : %d"), ItemIdx, ItemCount);

	pc->InventoryComponent->AddItem(ItemIdx, ItemCount);

	return;
}

void AItemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItemActor, ItemIdx);
	DOREPLIFETIME(AItemActor, ItemCount);
}