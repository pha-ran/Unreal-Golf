// Fill out your copyright notice in the Description page of Project Settings.


#include "FakeGolfBall.h"
#include "GolfBall.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AFakeGolfBall::AFakeGolfBall()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	//비주얼 표현을 담당할 메시 정의
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = StaticMesh;

	//사용할 메시 에셋이 발견되면 스태틱 메시와 위치/스케일 설정
	if (DefaultMesh.Succeeded())
	{
		StaticMesh->SetStaticMesh(DefaultMesh.Object);
		StaticMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -7.25f));
		StaticMesh->SetRelativeScale3D(FVector(0.125f, 0.125f, 0.125f));
	}

	StaticMesh->SetSimulatePhysics(false);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->bDoCollisionTest = false;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// 발사체 클래스 초기화
	static ConstructorHelpers::FClassFinder<APawn> GolfBallBP(TEXT("/Game/ThirdPerson/Blueprints/BP_GolfBall"));

	if (GolfBallBP.Class != NULL)
	{
		GolfBallBPClass = GolfBallBP.Class;
	}

	// 스윙 딜레이 초기화
	SwingRate = 1.0f;
	bIsSwung = false;

	// 세로 각도 초기화
	Angle = 0.0f;
	DeltaAngle = 2.5f;

	// 속도 초기화
	Speed = 0.0f;
	DeltaSpeed = 100.0f;
}

// Called when the game starts or when spawned
void AFakeGolfBall::BeginPlay()
{
	Super::BeginPlay();
	
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AFakeGolfBall::StartSwing()
{
	if (!bIsSwung)
	{
		bIsSwung = true;

		UWorld* World = GetWorld();
		World->GetTimerManager().SetTimer(SwingTimer, this, &AFakeGolfBall::StopSwing, SwingRate, false);

		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		SpawnProjectile(CameraRotation, Angle, Speed);
	}
}

void AFakeGolfBall::StopSwing()
{
	bIsSwung = false;
}

void AFakeGolfBall::SpawnProjectile_Implementation(FRotator _CameraRotation, double _Angle, float _Speed)
{
	FVector SpawnLocation = GetActorLocation() + FTransform(_CameraRotation).TransformVector(FVector(0.0f, 0.0f, 0.0f));
	FRotator SpawnRotation = _CameraRotation;

	SpawnRotation.Pitch += _Angle;

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = GetInstigator();
	SpawnParameters.Owner = this;

	AGolfBall* SpawnedProjectile = GetWorld()->SpawnActor<AGolfBall>(GolfBallBPClass, SpawnLocation, SpawnRotation, SpawnParameters);

	if (SpawnedProjectile)
	{
		FVector Direction = SpawnRotation.Vector();
		SpawnedProjectile->Setup(Direction, _Speed);

		APawn* Pawn = Cast<APawn>(SpawnedProjectile);

		if (Pawn != nullptr)
		{
			SetActorHiddenInGame(true);
			//GetController()->Possess(Pawn);
		}
	}
}

// Called every frame
void AFakeGolfBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFakeGolfBall::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AFakeGolfBall::Swing(const FInputActionValue& Value)
{
	bool IsPress = Value.Get<bool>();

	if (IsPress)
	{
		StartSwing();
	}
}

void AFakeGolfBall::Path(const FInputActionValue& Value)
{
	bool IsPress = Value.Get<bool>();

	if (IsPress)
	{
		UE_LOG(LogTemp, Display, TEXT("#################### Path ####################"));
	}
}

void AFakeGolfBall::AngleUp(const FInputActionValue& Value)
{
	bool IsPress = Value.Get<bool>();

	if (IsPress)
	{
		if (Angle < 60)
		{
			Angle += DeltaAngle;
		}
		else
		{
			Angle = 60.0f;
		}
	}
}

void AFakeGolfBall::AngleDown(const FInputActionValue& Value)
{
	bool IsPress = Value.Get<bool>();

	if (IsPress)
	{
		if (Angle > 0)
		{
			Angle -= DeltaAngle;
		}
		else
		{
			Angle = 0.0f;
		}
	}
}

void AFakeGolfBall::SpeedUp(const FInputActionValue& Value)
{
	bool IsPress = Value.Get<bool>();

	if (IsPress)
	{
		if (Speed < 3000)
		{
			Speed += DeltaSpeed;
		}
		else
		{
			Speed = 3000.0f;
		}
	}
}

void AFakeGolfBall::SpeedDown(const FInputActionValue& Value)
{
	bool IsPress = Value.Get<bool>();

	if (IsPress)
	{
		if (Speed > 0)
		{
			Speed -= DeltaSpeed;
		}
		else
		{
			Speed = 0.0f;
		}
	}
}

// Called to bind functionality to input
void AFakeGolfBall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFakeGolfBall::Look);

		//스윙
		EnhancedInputComponent->BindAction(SwingAction, ETriggerEvent::Triggered, this, &AFakeGolfBall::Swing);

		//예상 경로
		EnhancedInputComponent->BindAction(PathAction, ETriggerEvent::Triggered, this, &AFakeGolfBall::Path);

		//세로 각도 증가
		EnhancedInputComponent->BindAction(AngleUpAction, ETriggerEvent::Triggered, this, &AFakeGolfBall::AngleUp);

		//세로 각도 감소
		EnhancedInputComponent->BindAction(AngleDownAction, ETriggerEvent::Triggered, this, &AFakeGolfBall::AngleDown);

		//스윙 속도 증가
		EnhancedInputComponent->BindAction(SpeedUpAction, ETriggerEvent::Triggered, this, &AFakeGolfBall::SpeedUp);

		//스윙 속도 감소
		EnhancedInputComponent->BindAction(SpeedDownAction, ETriggerEvent::Triggered, this, &AFakeGolfBall::SpeedDown);
	}
}

