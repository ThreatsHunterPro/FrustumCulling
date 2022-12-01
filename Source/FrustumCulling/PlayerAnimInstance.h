#pragma once
#include "CoreMinimal.h"

#include "PlayerCharacter.h"

#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

UCLASS()
class FRUSTUMCULLING_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player anim instance values")
		float fVerticalValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player anim instance values")
		float fHorizontalValue = 0.0f;

private:
	virtual void NativeBeginPlay() override;
	void InitEvents(const APlayerCharacter* _player);

	UFUNCTION() void UpdateVerticalMovement(float _verticalValue);
	UFUNCTION() void UpdateHorizontalMovement(float _horizontalValue);
};