// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Week9Character.generated.h"


UCLASS(config=Game)
class AWeek9Character : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** ���� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SwingAction;

	/** ���� ��� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PathAction;

	/** ���� ���� ���� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AngleUpAction;

	/** ���� ���� ���� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AngleDownAction;

	/** ���� �ӵ� ���� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SpeedUpAction;

	/** ���� �ӵ� ���� */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SpeedDownAction;


public:
	AWeek9Character();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** ���� */
	void Swing(const FInputActionValue& Value);

	/** ���� ��� */
	void Path(const FInputActionValue& Value);

	/** ���� ���� ���� */
	void AngleUp(const FInputActionValue& Value);

	/** ���� ���� ���� */
	void AngleDown(const FInputActionValue& Value);

	/** ���� �ӵ� ���� */
	void SpeedUp(const FInputActionValue& Value);

	/** ���� �ӵ� ���� */
	void SpeedDown(const FInputActionValue& Value);
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Projectile")
	TSubclassOf<class AGolfBall> ProjectileClass;

	/** ���� ������, ������ ��. ���� �Լ��� �߰����� SpawnProjectile �� �Է¿� ���� ���ε����� �ʰ� �ϴ� ���� */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float SwingRate;

	/** true �� ��� �����ϴ� ���μ��� ���� */
	bool bIsSwung;

	/** ���� ���� �Լ� */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StartSwing();

	/** ���� ���� �Լ�. ȣ��Ǹ� �÷��̾ StartSwing �� �ٽ� ��� ���� */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StopSwing();

	/** �߻�ü�� �����ϴ� ���� �Լ� */
	UFUNCTION(Server, Reliable)
	void SpawnProjectile(double _Angle, float _Speed);

	/** ���� ���̿� ���� �����̸� �ִ� Ÿ�̸� �ڵ� */
	FTimerHandle SwingTimer;

	/** ���� ��ġ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector SpawnPosition;

	// ���� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	double Angle;

	// ���� ���� ��ȭ�� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	double DeltaAngle;

	// ���� �ӵ� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay)
	float Speed;

	// ���� �ӵ� ��ȭ�� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay)
	float DeltaSpeed;

};

