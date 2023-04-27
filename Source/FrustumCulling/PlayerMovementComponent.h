#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerMovementComponent.generated.h"

class UCameraComponent;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FRUSTUMCULLING_API UPlayerMovementComponent : public UActorComponent
{
	GENERATED_BODY()

	#pragma region Events

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMove, float, _value);

	UPROPERTY()
		FOnMove onMoveVertical = FOnMove();

	#pragma endregion
	
	UPROPERTY(EditAnywhere, Category = "Player values | Movement")
		bool bCanMove = true;

	UPROPERTY(VisibleAnywhere, Category = "Player values | Movement")
		bool bIsSprinting = false;
	
	UPROPERTY(EditAnywhere, Category = "Player values | Movement", meta = (ClampMin = "0.0", ClampMax = "25.0"))
		float fMoveSpeed = 6.0f;

	UPROPERTY(EditAnywhere, Category = "Player values | Movement", meta = (ClampMin = "15.0", ClampMax = "50.0"))
		float fSprintSpeed = 15.0f;

	UPROPERTY(EditAnywhere, Category = "Player values | Rotation")
		bool bCanRotate = true;

	UPROPERTY(EditAnywhere, Category = "Player values | Rotation", meta = (ClampMin = "-180.0", ClampMax = "180.0"))
		float fPitchMin = -15.0f;

	UPROPERTY(EditAnywhere, Category = "Player values | Rotation", meta = (ClampMin = "-180.0", ClampMax = "180.0"))
		float fPitchMax = 5.0f;
	
	UPROPERTY(EditAnywhere, Category = "Player values | Rotation", meta = (ClampMin = "0.0", ClampMax = "30.0"))
		float fRotateSpeed = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Player values | FOV", meta = (ClampMin = "-180.0", ClampMax = "180.0"))
		float fDefaultFOV = 90.0f;
	
	UPROPERTY(EditAnywhere, Category = "Player values | FOV", meta = (ClampMin = "-180.0", ClampMax = "180.0"))
		float fSprintFOV = 70.0f;
	
	UPROPERTY(EditAnywhere, Category = "Player values | FOV", meta = (ClampMin = "0.0", ClampMax = "30.0"))
		float fFOVSpeed = 10.0f;

	UPROPERTY(VisibleAnywhere, Category = "Player values | Pointers")
		AActor* owner = nullptr;
	
	UPROPERTY(VisibleAnywhere, Category = "Player values | Pointers")
		UCameraComponent* camera = nullptr;
	
public:
	FORCEINLINE FOnMove& OnMoveVertical() { return onMoveVertical; }
	
public:	
	UPlayerMovementComponent();

private:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public:
	UFUNCTION() void MoveVertical(float _value);
	UFUNCTION() void ToggleSprint();
	UFUNCTION() void MovePitch(float _value);
	UFUNCTION() void MoveYaw(float _value);
};