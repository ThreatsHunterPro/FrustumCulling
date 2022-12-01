#include "Octree.h"
#include "FrustumCullingGameModeBase.h"
#include "Kismet/GameplayStatics.h"

AOctree::AOctree()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

#pragma region Engine

void AOctree::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	const FVector& _scale = boxScale / 2.0f;
	const FVector& _min = GetActorLocation() - _scale;
	const FVector& _max = GetActorLocation() + _scale;
	box = FBox(_min, _max);
}

void AOctree::BeginPlay()
{
	if (iAccuracy == 1)
	{
		AFrustumCullingGameModeBase* _gameMode = GetWorld()->GetAuthGameMode<AFrustumCullingGameModeBase>();
		if (!_gameMode) return;

		_gameMode->SetOctree(this);
	}
}

void AOctree::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	DrawOctrees();
}

bool AOctree::ShouldTickIfViewportsOnly() const
{
	return bShowDebug;
}

#pragma endregion 

#pragma region Tool

void AOctree::Update()
{
	// Clear all children
	Clear();
	
	// Get all meshes in the world
	GetActors();

	// If there is to many meshes, subdivide them in different octree
	if (actors.Num() > iCapacity)
	{
		SpawnChildren();
	}

	else if (master)
	{
		master->AddOctreeWithoutChildren(this);
	}
}

void AOctree::SwitchVisibility()
{
	// Switch current visibility
	bShowDebug = !bShowDebug;

	// Check if this octree has children
	if (HasChildren())
	{
		// Iterate all children
		const int& _childrenCount = octreeChildren.Num();
		for (int _childIndex(0); _childIndex < _childrenCount; _childIndex++)
		{
			// Get the current octree
			AOctree* _octree = octreeChildren[_childIndex];
			if (!_octree) continue;
			
			// Change his visibility
			_octree->SwitchVisibility();
		}
	}
}

void AOctree::Clear()
{
	// Do this only for the first one
	if (iAccuracy == 1)
	{
		actors.Empty();
		octreesWithoutChildren.Empty();
		master = this;
	}
	
	// Reset all octrees and meshes
	ResetOctree();
}

#pragma endregion

void AOctree::DrawOctrees() const
{
	if (!bShowDebug) return;
	
	// Draw current octree as box
	DrawDebugBox(GetWorld(), box.GetCenter(), box.GetExtent(), lineColor, false, -1, 0, fThickness);

	// Draw a solid box if actors are invisible
	if (bIsHidingActors)
	{
		DrawDebugSolidBox(GetWorld(), box.GetCenter(), box.GetExtent(), debugColor.WithAlpha(20), false, -1.0f, 0);
	}
}

void AOctree::GetActors()
{
	TArray<AActor*> _allActors;
	
	// Do this only for the first one
	if (iAccuracy <= 1)
	{
		// Get all actors in the world
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), _allActors);
	}
	
	// Otherwise get all actors of the owner
	else
	{
		// Get all parent's actors before empty it
		const int& _actorsCount = actors.Num();
		for	(int _actorIndex(0); _actorIndex < _actorsCount; _actorIndex++)
		{
			TSoftObjectPtr<AActor> _actor = actors[_actorIndex];
			if (!_actor) continue;
			_allActors.Add(_actor.Get());
		}

		// Reset his actors before filling it again
		actors.Empty();
	}

	// Iterate all actors
	const int& _actorsCount = _allActors.Num();
	for	(int _actorIndex(0); _actorIndex < _actorsCount; _actorIndex++)
	{
		// Get the current actor
		const AActor* _actor = _allActors[_actorIndex];
		if (!_actor) continue;

		// Check if this actor can't be hidden
		if (actorsToIgnore.Contains(_actor)) continue;
		
		// Check if this actor is into the current Octree
		const FVector& _actorLocation = _actor->GetActorLocation();
		
		// const FBox& _actorBoundingBox = _actor->GetStreamingBounds();
		if (!box.IsInside(_actorLocation)) continue;

		// Check if this actor contains a static mesh
		UActorComponent* _component = _actor->GetComponentByClass(UStaticMeshComponent::StaticClass());
		if (!Cast<UStaticMeshComponent>(_component)) continue;

		// Then add it to actors
		actors.Add(_actor);
	}
}

void AOctree::SpawnChildren()
{
	if (!IsValid(octreeType))
	{
		UE_LOG(LogTemp, Warning, TEXT("You have forgot to set an OctreeType !"))
		return;
	}
	
	// Iterate 8 times (=> Octree)
	for (int _childIndex(0); _childIndex < 8; _childIndex++)
	{
		// Spawn a new octree
		AOctree* _child = GetWorld()->SpawnActor<AOctree>(octreeType);
		if (!_child) continue;
		_child->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		
		// Get the center and extent of the current bounding box
		FVector _initialExtent = box.GetExtent(), _extent = box.GetExtent();
		
		// Init the new octree's values
		_extent.X = _childIndex % 2 == 0 ? 0 :_extent.X;
		_extent.Y = _childIndex % 4 >= 2 ? _extent.Y : 0;
		_extent.Z = _childIndex >= 4 ? _extent.Z : 0;
		
		const FVector& _min = box.Min + _extent;
		const FVector& _max = _min + _initialExtent;
		const FBox& _boundingBox = FBox(_min, _max);
		const int& _accuracy = iAccuracy + 1;

		_child->Setup(_accuracy, FBox(_boundingBox), master, actors);

		// Add it to the children
		octreeChildren.Add(_child);
		
		// Init the new octree
		_child->Update();
	}
}

void AOctree::ResetOctree()
{
	// Iterate all children
	const int& _childCount = octreeChildren.Num();
	for (int _childIndex(0); _childIndex < _childCount; _childIndex++)
	{
		// Get each children
		AOctree* _octree = octreeChildren[_childIndex]; 
		if (!_octree) continue;
		
		// Destroy children
		_octree->ResetOctree();
		
		// Destroy this octree
		_octree->Destroy();
	}

	// Clear all actors and children stored 
	octreeChildren.Empty();
}

void AOctree::ChangeActorsVisibility(bool _visible)
{
	// Check if the visibility gonna change
	if (bIsHidingActors != _visible) return;

	// Set his current visibility
	bIsHidingActors = !_visible;

	// Goes through all the actors contained
	const int& _actorsCount = actors.Num();
	for (int _actorIndex(0); _actorIndex < _actorsCount; _actorIndex++)
	{
		// Get each actor
		const AActor* _actor = actors[_actorIndex].Get();
		if (!_actor) continue;
		
		// Get his SceneComponent to change the visibility
		USceneComponent* _sceneComponent = _actor->GetRootComponent();
		if (!_sceneComponent) continue;
		_sceneComponent->SetVisibility(!bIsHidingActors);
	}
}