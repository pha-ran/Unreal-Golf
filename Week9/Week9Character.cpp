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

	// �߻�ü Ŭ���� �ʱ�ȭ
	ProjectileClass = AGolfBall::StaticClass();

	// ���� ������ �ʱ�ȭ
	SwingRate = 1.0f;
	bIsSwung = false;

	// ���� ��ġ �ʱ�ȭ
	SpawnPosition = FVector(0.0f, 100.0f, -100.0f);

	// ���� ���� �ʱ�ȭ
	Angle = 0.0f;
	DeltaAngle = 2.5f;

	// �ӵ� �ʱ�ȭ
	Speed = 0.0f;
	DeltaSpeed = 100.0f;
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

		UE_LOG(LogTemp, Display, TEXT("Angle %f / Speed %f"), _Angle, _Speed);
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

		//����
		EnhancedInputComponent->BindAction(SwingAction, ETriggerEvent::Triggered, this, &AWeek9Character::Swing);

		//���� ���
		EnhancedInputComponent->BindAction(PathAction, ETriggerEvent::Triggered, this, &AWeek9Character::Path);

		//���� ���� ����
		EnhancedInputComponent->BindAction(AngleUpAction, ETriggerEvent::Triggered, this, &AWeek9Character::AngleUp);

		//���� ���� ����
		EnhancedInputComponent->BindAction(AngleDownAction, ETriggerEvent::Triggered, this, &AWeek9Character::AngleDown);

		//���� �ӵ� ����
		EnhancedInputComponent->BindAction(SpeedUpAction, ETriggerEvent::Triggered, this, &AWeek9Character::SpeedUp);

		//���� �ӵ� ����
		EnhancedInputComponent->BindAction(SpeedDownAction, ETriggerEvent::Triggered, this, &AWeek9Character::SpeedDown);
	}

}

void AWeek9Character::Move(const FInputActionValue& Value)
{
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
	bool IsPress = Value.Get<bool>();

	if (IsPress)
	{
		StartSwing();
	}
}

void AWeek9Character::Path(const FInputActionValue& Value)
{
	bool IsPress = Value.Get<bool>();

	if (IsPress)
	{
		UE_LOG(LogTemp, Display, TEXT("#################### Path ####################"));
	}
}

void AWeek9Character::AngleUp(const FInputActionValue& Value)
{
	bool IsPress = Value.Get<bool>();

	if (IsPress)
	{
		if (Angle <= 45)
		{
			Angle += DeltaAngle;
		}
		else
		{
			Angle = 80.0f;
		}
	}
}

void AWeek9Character::AngleDown(const FInputActionValue& Value)
{
	bool IsPress = Value.Get<bool>();

	if (IsPress)
	{
		if (Angle >= 0)
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
	bool IsPress = Value.Get<bool>();

	if (IsPress)
	{
		if (Speed <= 3000)
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
	bool IsPress = Value.Get<bool>();

	if (IsPress)
	{
		if (Speed >= 0)
		{
			Speed -= DeltaSpeed;
		}
		else
		{
			Speed = 0.0f;
		}
	}
}

