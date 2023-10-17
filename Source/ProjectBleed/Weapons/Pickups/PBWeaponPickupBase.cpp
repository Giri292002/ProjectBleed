// All Rights belong to Studio WASD 2023


#include "PBWeaponPickupBase.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

// Sets default values
APBWeaponPickupBase::APBWeaponPickupBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereOverlap = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Overlap"));
	SetRootComponent(SphereOverlap);
	SphereOverlap->SetGenerateOverlapEvents(true);
	SphereOverlap->SetCollisionProfileName(TEXT("Trigger"));
	SphereOverlap->OnComponentBeginOverlap.AddDynamic(this, &APBWeaponPickupBase::OnComponentOverlap);
	
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

// Called every frame
void APBWeaponPickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APBWeaponPickupBase::OnComponentOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Only players can overlap.
	//TODO: Make sure we use tags instead of casting

	if (Cast<APBCharacter>(OtherActor))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Overlapped")));
	}
}

