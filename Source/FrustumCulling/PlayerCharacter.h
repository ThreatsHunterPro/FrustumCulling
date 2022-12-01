#pragma once
#include "CoreMinimal.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UPlayerMovementComponent;
class UPlayerAnimInstance;

UCLASS()
class FRUSTUMCULLING_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	#pragma region Components
	
	UPROPERTY(EditAnywhere, Category = "Player values | Components")
		USpringArmComponent* springArm = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Player values | Components")
		UCameraComponent* camera = nullptr;

	UPROPERTY(EditAnywhere, Category = "Player values | Components")
		UPlayerMovementComponent* movement = nullptr;

	UPROPERTY(EditAnywhere, Category = "Player values | Components")
		UPlayerAnimInstance* animInstance = nullptr;

	#pragma endregion
	
public:
	#pragma region Getters
	
	FORCEINLINE UPlayerMovementComponent* GetPlayerMovement() const { return movement; }
	
	#pragma endregion
	
public:
	APlayerCharacter();

private:
	#pragma region Engine
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	#pragma endregion
};