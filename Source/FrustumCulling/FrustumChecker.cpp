#include "FrustumChecker.h"
#include "FrustumCullingGameModeBase.h"
#include "Components/BrushComponent.h"

UFrustumChecker::UFrustumChecker()
{
	PrimaryComponentTick.bCanEverTick = false;
}
void UFrustumChecker::BeginPlay()
{
	Super::BeginPlay();

	world = GetWorld();
	if (!world) return;
	
	if (AFrustumCullingGameModeBase* _gameMode = world->GetAuthGameMode<AFrustumCullingGameModeBase>())
	{
		octree = _gameMode->GetOctree();
		if (octree)
		{
			InitTimer(octree);
			return;
		}
		
		_gameMode->OnOctreeSetted()->AddDynamic(this, &UFrustumChecker::InitTimer);
	}
}

void UFrustumChecker::InitTimer(AOctree* _octree)
{
	octree = _octree;
	if (!world) return;

	localPlayer = world->GetFirstLocalPlayerFromController();
	if (localPlayer)
	{
		viewportClient = localPlayer->ViewportClient;
		if (localPlayer)
		{
			viewport = viewportClient->Viewport;
		}
	}
	
	FTimerHandle checkTimer = FTimerHandle();
	world->GetTimerManager().SetTimer(checkTimer, this, &UFrustumChecker::CheckVisibility, fCheckRate, true, fCheckRate);
}

void UFrustumChecker::CheckVisibility()
{
	if (!octree) return;

	// Update the scene view
	UpdateSceneView();
	
	// Get all actors from the stored Octree
	TArray<TSoftObjectPtr<AActor>> _actors = octree->GetAllActors();
	const int& _actorsCount = _actors.Num();

	// Run though all actors
	for (int _actorIndex(0); _actorIndex < _actorsCount; _actorIndex++)
	{
		// Get each actor
		const AActor* _actor = _actors[_actorIndex].Get();
		if (!_actor) continue;

		// Only use for range system
		bool _isInRange = false;

		// If we use the range system
		if (bUseRange)
		{
			// Check the distance with the current actor, if he is in the set range then it will be automatically displayed
			_isInRange = FVector::Distance(GetOwner()->GetActorLocation(), _actor->GetActorLocation()) <= fRange;
		}
		
		// Get his SceneComponent
		USceneComponent* _sceneComponent = _actor->GetRootComponent();
		if (!_sceneComponent) continue;

		// If he his in the camera's frustum, hide him and his children
		_sceneComponent->SetVisibility(_isInRange ? true : IsInFrustum(_actor), true);
	}
}
void UFrustumChecker::UpdateSceneView()
{
	// Check necessary pointers
	if (!localPlayer || ! viewportClient || !viewport) return;

	// Compute the scene view
	FSceneViewFamilyContext _viewFamily(FSceneViewFamily::ConstructionValues(viewport, world->Scene, viewportClient->EngineShowFlags).SetRealtimeUpdate(true));
	FVector _viewLocation;
	FRotator _viewRotation;
	sceneView = localPlayer->CalcSceneView(&_viewFamily, _viewLocation, _viewRotation, viewport);
}
bool UFrustumChecker::IsInFrustum(const AActor* _target) const
{
	if (!_target || !sceneView) return false;

	// Check if the target's bounding box is contained into the view frustum
	const FBox& _box = _target->GetComponentsBoundingBox();
	const FConvexVolume _volume = sceneView->ViewFrustum;
	
	// The param 'fCheckScale' anticipates potential Z-axis rotations
	return _volume.IntersectBox(_box.GetCenter(), _box.GetExtent() * fCheckScale);
}