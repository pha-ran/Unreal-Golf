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

	/** 스윙 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SwingAction;

	/** 예상 경로 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PathAction;

	/** 세로 각도 증가 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AngleUpAction;

	/** 세로 각도 감소 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AngleDownAction;

	/** 스윙 속도 증가 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SpeedUpAction;

	/** 스윙 속도 감소 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SpeedDownAction;

protected:
	// 오브젝트의 비주얼 표현을 제공
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* StaticMesh;

public:
	// Sets default values for this pawn's properties
	AFakeGolfBall();

protected:
	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** 스윙 */
	void Swing(const FInputActionValue& Value);

	/** 예상 경로 */
	void Path(const FInputActionValue& Value);

	/** 세로 각도 증가 */
	void AngleUp(const FInputActionValue& Value);

	/** 세로 각도 감소 */
	void AngleDown(const FInputActionValue& Value);

	/** 스윙 속도 증가 */
	void SpeedUp(const FInputActionValue& Value);

	/** 스윙 속도 감소 */
	void SpeedDown(const FInputActionValue& Value);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	TSubclassOf<APawn> GolfBallBPClass;

	/** 스윙 딜레이, 단위는 초. 서버 함수의 추가분이 SpawnProjectile 을 입력에 직접 바인딩하지 않게 하는 역할 */
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float SwingRate;

	/** true 인 경우 스윙하는 프로세스 도중 */
	bool bIsSwung;

	/** 스윙 시작 함수 */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StartSwing();

	/** 스윙 종료 함수. 호출되면 플레이어가 StartSwing 을 다시 사용 가능 */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StopSwing();

	/** 발사체를 스폰하는 서버 함수 */
	UFUNCTION(Server, Reliable)
	void SpawnProjectile(FRotator _CameraRotation, double _Angle, float _Speed);

	/** 스폰 사이에 스윙 딜레이를 넣는 타이머 핸들 */
	FTimerHandle SwingTimer;

	// 세로 각도 저장
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	double Angle;

	// 세로 각도 변화량 저장
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	double DeltaAngle;

	// 스윙 속도 저장
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay)
	float Speed;

	// 스윙 속도 변화량 저장
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
