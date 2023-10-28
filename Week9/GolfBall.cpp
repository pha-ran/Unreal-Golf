// Fill out your copyright notice in the Description page of Project Settings.


#include "GolfBall.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AGolfBall::AGolfBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bReplicates = true;

	//발사체와 콜리전의 루트 컴포넌트 역할을 할 스피어 컴포넌트 정의
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	SphereComponent->InitSphereRadius(6.25f);
	SphereComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	RootComponent = SphereComponent;

	//비주얼 표현을 담당할 스태틱 메시 정의
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(RootComponent);

	//사용할 메시 에셋이 발견되면 스태틱 메시의 위치/스케일 설정
	if (DefaultMesh.Succeeded())
	{
		StaticMesh->SetStaticMesh(DefaultMesh.Object);
		StaticMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -6.25f));
		StaticMesh->SetRelativeScale3D(FVector(0.125f, 0.125f, 0.125f));
	}

	//발사체 무브먼트 컴포넌트 정의
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->SetUpdatedComponent(SphereComponent);

	if (HasAuthority())
	{
		ProjectileMovementComponent->InitialSpeed = 2000.0f;
		ProjectileMovementComponent->MaxSpeed = 3000.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->ProjectileGravityScale = 1.0f;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->bSimulationEnabled = true;
		ProjectileMovementComponent->Bounciness = 0.3;
		ProjectileMovementComponent->Friction = 0.1f;
		ProjectileMovementComponent->BounceVelocityStopSimulatingThreshold = 0.0f;
	}
}

// Called when the game starts or when spawned
void AGolfBall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGolfBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGolfBall::Setup(FVector& Direction, float Speed)
{
	ProjectileMovementComponent->Velocity = Direction * (ProjectileMovementComponent->InitialSpeed + Speed);
}

