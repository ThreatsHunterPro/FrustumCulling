#pragma once
#include "CoreMinimal.h"

#include "Octree.h"

#include "Components/ActorComponent.h"
#include "FrustumChecker.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FRUSTUMCULLING_API UFrustumChecker : public UActorComponent
{
	GENERATED_BODY()

	#pragma region Values

	/* Should it use range for checking */
	UPROPERTY(EditAnywhere, Category = "Frutum checker values")
		bool bUseRange = true;

	/* Range of checker detection */
	UPROPERTY(EditAnywhere, Category = "Frutum checker values", meta = (UIMin = "0.0", UIMax = "100000.0", ClampMin = "0.0", ClampMax = "100000.0", EditCondition = "bUseRange", EditConditionHides))
		float fRange = 500.0f;
	
	/* Rate of the detection */
	UPROPERTY(EditAnywhere, Category = "Frutum checker values", meta = (UIMin = "0.1", UIMax = "60.0", ClampMin = "0.1", ClampMax = "60.0"))
		float fCheckRate = 0.1f;

	/* Rate of the detection */
	UPROPERTY(EditAnywhere, Category = "Frutum checker values", meta = (UIMin = "0.1", UIMax = "1000.0", ClampMin = "0.1", ClampMax = "1000.0"))
		float fCheckScale = 10.0f;

	#pragma endregion

	#pragma region Pointers

	/* Current world */
	UPROPERTY()
		UWorld* world = nullptr;

	/* Current octree */
	UPROPERTY()
		AOctree* octree = nullptr;

	/* Current local player */
	UPROPERTY()
		ULocalPlayer* localPlayer = nullptr;

	/* Current viewportClient */
	UPROPERTY()
		TObjectPtr<UGameViewportClient> viewportClient = nullptr;

	/* Current SceneView */
		FSceneView* sceneView = nullptr;
	
	/* Current viewport */
		FViewport* viewport = nullptr;

	#pragma endregion

public:	
	UFrustumChecker();

	#pragma region Engine
	
private:
	virtual void BeginPlay() override;

	#pragma endregion

	/* When the octree is set, start a timer for checking method */
	UFUNCTION() void InitTimer(AOctree* _octree);

	/* Run through all actors contained into the current octree */
	UFUNCTION() void CheckVisibility();

	/* Update the current scene view of the renderer */
	void UpdateSceneView();

	/* Check if the current actor volume is into the camera's frustum */
	bool IsInFrustum(const AActor* _target) const;
};