#include "FrustumChecker.h"
#include "FrustumCullingGameModeBase.h"

UFrustumChecker::UFrustumChecker()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFrustumChecker::BeginPlay()
{
	Super::BeginPlay();
			
	// if (const ACamera* _camera = Cast<ACamera>(GetOwner()))
	// {
	// 	cameraComponent = _camera->GetCameraComponent();
	// }

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
	world->GetTimerManager().SetTimer(checkTimer, this, &UFrustumChecker::CheckOctree, fCheckRate, true, fCheckRate);
}

void UFrustumChecker::CheckOctree()
{
	if (!octree) return;

	const TArray<AOctree*>& _octrees = octree->GetOctreesWithoutChildren();
	const int& _octreesCount = _octrees.Num();
	
	for (int _index(0); _index < _octreesCount; _index++)
	{
		AOctree* _octree = _octrees[_index];
		if (!_octree) return;
		
		// Check if it's into the frustum
		const bool _isInFrustum = IsInFrustum(_octree);

		// Update the visibility of the actors
		_octree->ChangeActorsVisibility(_isInFrustum);
	}
}

bool UFrustumChecker::IsInFrustum(const AOctree* _target) const
{
	if (!localPlayer || ! viewportClient || !viewport) return false;
	
	FSceneViewFamilyContext _viewFamily(FSceneViewFamily::ConstructionValues(viewport, world->Scene, viewportClient->EngineShowFlags).SetRealtimeUpdate(true));

	FVector _viewLocation;
	FRotator _viewRotation;
	if (const FSceneView* _sceneView = localPlayer->CalcSceneView(&_viewFamily, _viewLocation, _viewRotation, viewport))
	{
		const FBox& _box = _target->GetBox();
		const bool _isInFrustum = _sceneView->ViewFrustum.IntersectBox(_box.GetCenter(), _box.GetExtent());
		
		const FVector& _targetLocation = _target->GetActorLocation();
		const bool _isInRange = _sceneView->ViewFrustum.DistanceTo(_targetLocation) <= fRange;
		
		return _isInFrustum && _isInRange;
	}

	return false;
}