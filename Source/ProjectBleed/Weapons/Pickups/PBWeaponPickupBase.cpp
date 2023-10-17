// All Rights belong to Studio WASD 2023


#include "PBWeaponPickupBase.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "../PBWeaponBase.h"
#include "ProjectBleed/Libraries/CustomLogging.h"

// Sets default values
APBWeaponPickupBase::APBWeaponPickupBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereOverlap = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Overlap"));
	SetRootComponent(SphereOverlap);
	SphereOverlap->SetGenerateOverlapEvents(true);
	SphereOverlap->SetCollisionProfileName(TEXT("Interactable"));
	
	WeaponStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponStaticMesh->SetCollisionProfileName(TEXT("NoCollision"));
	WeaponStaticMesh->SetupAttachment(GetRootComponent());

	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	SphereMesh->SetupAttachment(GetRootComponent());
	ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (SphereMeshAsset.Object != NULL)
	{
		SphereMesh->SetStaticMesh(SphereMeshAsset.Object);
	}
	SphereMesh->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void APBWeaponPickupBase::BeginPlay()
{
	Super::BeginPlay();
	
}

int APBWeaponPickupBase::GetInteractionPriority_Implementation()
{
	return InteractionPriority;
}

void APBWeaponPickupBase::Interact_Implementation(AActor* Interactor)
{
	UE_LOG(LogPBWeapon, Log, TEXT("Interacted"));
	//Makes sure that interaction component doesn't interact with this component again
	InteractionPriority = -1;

	APBCharacter* PBCharacterOwner = Cast<APBCharacter>(Interactor);

	if (!IsValid(PBCharacterOwner))
	{
		V_LOG_ERROR(LogPBWeapon, TEXT("Invalid PBCharacter Owner"));
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Interactor;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (WeaponToGive == nullptr)
	{
		V_LOG_ERROR(LogPBWeapon, TEXT("Invalid Weapon To Give Class"));
		return;
	}

	APBWeaponBase* SpawnedPBWeapon = Cast<APBWeaponBase>(GetWorld()->SpawnActor<APBWeaponBase>(WeaponToGive, GetActorLocation(), GetActorRotation(), SpawnParams));
	if (!IsValid(SpawnedPBWeapon))
	{
		V_LOG_ERROR(LogPBWeapon, TEXT("Invalid SpawnedPBWeapon"));
		return;
	}

	const FAttachmentTransformRules& AttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false);

	SpawnedPBWeapon->AttachToComponent(PBCharacterOwner->GetMesh(), AttachmentRules, FName(TEXT("WeaponSocket")));
	
}
// Called every frame
void APBWeaponPickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

