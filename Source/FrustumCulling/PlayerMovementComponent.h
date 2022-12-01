#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerMovementComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FRUSTUMCULLING_API UPlayerMovementComponent : public UActorComponent
{
	GENERATED_BODY()

	#pragma region Events

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMove, float, _value);

	UPROPERTY()
		FOnMove onMoveVertical = FOnMove();

	UPROPERTY()
		FOnMove onMoveHorizontal = FOnMove();

	#pragma endregion
	
	UPROPERTY(EditAnywhere, Category = "Player values | Movement")
		bool bCanMove = true;

	UPROPERTY(EditAnywhere, Category = "Player values | Rotation")
		bool bCanRotate = true;
	
	UPROPERTY(EditAnywhere, Category = "Player values | Movement", meta = (ClampMin = "0.0", ClampMax = "25.0"))
		float fMoveSpeed = 6.0f;

	UPROPERTY(EditAnywhere, Category = "Player values | Rotation", meta = (ClampMin = "-180.0", ClampMax = "180.0"))
		float fPitchMin = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Player values | Rotation", meta = (ClampMin = "-180.0", ClampMax = "180.0"))
		float fPitchMax = 0.0f;
	
	UPROPERTY(EditAnywhere, Category = "Player values | Rotation", meta = (ClampMin = "0.0", ClampMax = "30.0"))
		float fRotateSpeed = 10.0f;
	
	UPROPERTY(VisibleAnywhere, Category = "Player values | Movement")
		FVector location = FVector(0.0f);
	
	UPROPERTY(VisibleAnywhere, Category = "Player values | Rotation")
		FRotator rotation = FRotator(0.0f);
	
public:
	FORCEINLINE FOnMove& OnMoveVertical() { return onMoveVertical; }
	FORCEINLINE FOnMove& OnMoveHorizontal() { return onMoveHorizontal; }
	
public:	
	UPlayerMovementComponent();

private:
	#pragma region Engine
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	#pragma endregion

public:
	UFUNCTION() void MoveVertical(float _value);
	UFUNCTION() void MoveHorizontal(float _value);
	UFUNCTION() void MoveYaw(float _value);
	UFUNCTION() void MovePitch(float _value);
};