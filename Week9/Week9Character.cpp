// Copyright Epic Games, Inc. All Rights Reserved.

#include "Week9Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "GolfBall.h"
#include "Week9GameMode.h"
#include "Week9PlayerState.h"
#include "Week9GameState.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


//////////////////////////////////////////////////////////////////////////
// AWeek9Character

AWeek9Character::AWeek9Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	// 발사체 클래스 초기화
	ProjectileClass = AGolfBall::StaticClass();

	// 스윙 딜레이 초기화
	SwingRate = 1.0f;
	bIsSwung = false;

	// 생성 위치 초기화
	SpawnPosition = FVector(0.0f, 100.0f, -100.0f);

	// 세로 각도 초기화
	Angle = 0.0f;
	DeltaAngle = 2.5f;

	// 속도 초기화
	Speed = 0.0f;
	DeltaSpeed = 100.0f;

	// 입력 초기화
	IgnoreInputAll = true;

	// 시작 위치 초기화
	NextPositionX = 3836.0f;
	NextPositionY = -21230.0f;
	NextPositionZ = 193.0f;
}

void AWeek9Character::BeginPlay()
{
	// Call the base class  
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

void AWeek9Character::StartSwing()
{
	if (!bIsSwung)
	{
		bIsSwung = true;

		UWorld* World = GetWorld();
		World->GetTimerManager().SetTimer(SwingTimer, this, &AWeek9Character::StopSwing, SwingRate, false);

		SpawnProjectile(Angle, Speed);
	}
}

void AWeek9Character::StopSwing()
{
	bIsSwung = false;
}

void AWeek9Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeek9Character, IgnoreInputAll);

	DOREPLIFETIME(AWeek9Character, NextPositionX);
	DOREPLIFETIME(AWeek9Character, NextPositionY);
	DOREPLIFETIME(AWeek9Character, NextPositionZ);
}

void AWeek9Character::ServerNextTurn_Implementation()
{
	if (HasAuthority())
	{
		MulticastNextTurn();
	}
}

void AWeek9Character::MulticastNextTurn_Implementation()
{
	if (HasAuthority())
	{
		UWorld* MyWorld = GetWorld();

		if (MyWorld != nullptr)
		{
			AWeek9GameMode* MyGameMode = Cast<AWeek9GameMode>(MyWorld->GetAuthGameMode());
			AWeek9PlayerState* WPS = Cast<AWeek9PlayerState>(Controller->PlayerState);

			if (MyGameMode != nullptr && WPS != nullptr)
			{
				AWeek9GameState* GameState = Cast<AWeek9GameState>(MyWorld->GetGameState());

				if (GameState != nullptr)
				{
					if (GameState->GetCurrentTurn() == WPS->GetPlayerTurn())
					{
						MyGameMode->NextTurn();
					}
				}
			}
		}
	}
}

void AWeek9Character::SetIgnoreInputAll(bool _Ignore)
{
	if (HasAuthority())
	{
		IgnoreInputAll = _Ignore;
	}
}

void AWeek9Character::SetNextPosition(FVector _Position)
{
	if (HasAuthority())
	{
		NextPositionX = _Position.X;
		NextPositionY = _Position.Y;
		NextPositionZ = _Position.Z;
	}
}

void AWeek9Character::SpawnProjectile_Implementation(double _Angle, float _Speed)
{
	FVector CameraLocation;
	FRotator CameraRotation;
	GetActorEyesViewPoint(CameraLocation, CameraRotation);

	FVector spawnLocation = CameraLocation + FTransform(CameraRotation).TransformVector(SpawnPosition);
	FRotator spawnRotation = CameraRotation;

	spawnRotation.Pitch += _Angle;

	FActorSpawnParameters spawnParameters;
	spawnParameters.Instigator = GetInstigator();
	spawnParameters.Owner = this;

	AGolfBall* spawnedProjectile = GetWorld()->SpawnActor<AGolfBall>(spawnLocation, spawnRotation, spawnParameters);

	if (spawnedProjectile)
	{
		FVector Direction = spawnRotation.Vector();
		spawnedProjectile->Setup(Direction, _Speed);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AWeek9Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWeek9Character::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWeek9Character::Look);

		//스윙
		EnhancedInputComponent->BindAction(SwingAction, ETriggerEvent::Triggered, this, &AWeek9Character::Swing);

		//예상 경로
		EnhancedInputComponent->BindAction(PathAction, ETriggerEvent::Triggered, this, &AWeek9Character::Path);

		//세로 각도 증가
		EnhancedInputComponent->BindAction(AngleUpAction, ETriggerEvent::Triggered, this, &AWeek9Character::AngleUp);

		//세로 각도 감소
		EnhancedInputComponent->BindAction(AngleDownAction, ETriggerEvent::Triggered, this, &AWeek9Character::AngleDown);

		//스윙 속도 증가
		EnhancedInputComponent->BindAction(SpeedUpAction, ETriggerEvent::Triggered, this, &AWeek9Character::SpeedUp);

		//스윙 속도 감소
		EnhancedInputComponent->BindAction(SpeedDownAction, ETriggerEvent::Triggered, this, &AWeek9Character::SpeedDown);
	}

}

void AWeek9Character::Move(const FInputActionValue& Value)
{
	if (IgnoreInputAll)
	{
		return;
	}

	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AWeek9Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		//AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AWeek9Character::Swing(const FInputActionValue& Value)
{
	if (IgnoreInputAll)
	{
		return;
	}

	bool IsPress = Value.Get<bool>();

	if (IsPress)
	{
		StartSwing();

		IgnoreInputAll = true;
	}
}

void AWeek9Character::Path(const FInputActionValue& Value)
{
	if (IgnoreInputAll)
	{
		return;
	}

	bool IsPress = Value.Get<bool>();

	if (IsPress)
	{
		UE_LOG(LogTemp, Display, TEXT("#################### Path ####################"));
	}
}

void AWeek9Character::AngleUp(const FInputActionValue& Value)
{
	if (IgnoreInputAll)
	{
		return;
	}

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

void AWeek9Character::AngleDown(const FInputActionValue& Value)
{
	if (IgnoreInputAll)
	{
		return;
	}

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

void AWeek9Character::SpeedUp(const FInputActionValue& Value)
{
	if (IgnoreInputAll)
	{
		return;
	}

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

void AWeek9Character::SpeedDown(const FInputActionValue& Value)
{
	if (IgnoreInputAll)
	{
		return;
	}

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

FVector AWeek9Character::GetNextPosition()
{
	return FVector(NextPositionX, NextPositionY, NextPositionZ);
}

double AWeek9Character::GetAngle()
{
	return Angle;
}

float AWeek9Character::GetSpeed()
{
	return Speed;
}
