// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "FakeGolfBall.generated.h"

UCLASS()
class WEEK10_API AFakeGolfBall : public APawn
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

protected:
	// ������Ʈ�� ���־� ǥ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* StaticMesh;

public:
	// Sets default values for this pawn's properties
	AFakeGolfBall();

protected:
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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	TSubclassOf<APawn> GolfBallBPClass;

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
	void SpawnProjectile(FRotator _CameraRotation, double _Angle, float _Speed);

	/** ���� ���̿� ���� �����̸� �ִ� Ÿ�̸� �ڵ� */
	FTimerHandle SwingTimer;

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

};
