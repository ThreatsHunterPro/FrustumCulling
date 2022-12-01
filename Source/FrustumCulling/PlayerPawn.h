#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerPawn.generated.h"

UCLASS()
class FRUSTUMCULLING_API APlayerPawn : public APawn
{
	GENERATED_BODY()

	#pragma region Components

	UPROPERTY(EditAnywhere, Category = "Player values | Components")
		UCapsuleComponent* capsule = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Player values | Components")
		USkeletalMeshComponent* skeletal = nullptr;

	UPROPERTY(EditAnywhere, Category = "Player values | Components")
		USpringArmComponent* springArm = nullptr;

	UPROPERTY(EditAnywhere, Category = "Player values | Components")
		UCameraComponent* camera = nullptr;

	#pragma endregion

	#pragma region Movement

	UPROPERTY(EditAnywhere, Category = "Player values | Movement")
		bool bCanMove = true;

	UPROPERTY(EditAnywhere, Category = "Player values | Movement")
		bool bCanRotate = true;
	
	UPROPERTY(EditAnywhere, Category = "Player values | Movement", meta = (ClampMin = "0.0", ClampMax = "25.0"))
		float fMoveSpeed = 6.0f;

	UPROPERTY(EditAnywhere, Category = "Player values | Movement", meta = (ClampMin = "0.0", ClampMax = "30.0"))
		float fRotateSpeed = 10.0f;

	#pragma endregion
	
public:
	APlayerPawn();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION() void MoveVertical(float _value);
	UFUNCTION() void MoveHorizontal(float _value);
	void MoveYaw(float _value);
	void MovePitch(float _value);
};