// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/UnitGrid.h"

#include "Actors/BaseUnit.h"
#include "Actors/UnitCell.h"
#include "Events/ClientGameEventManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AUnitGrid::AUnitGrid()
{
	Rows = 5;
	Columns = 4;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	LastHighlightedCell = nullptr;
	bReplicates = true;
}

// Called when the game starts or when spawned
void AUnitGrid::BeginPlay()
{
	Super::BeginPlay();
	// Server Initialize only
	if (HasAuthority())
	{
		InitializeGrid();
	}
	else
	{
		// Register Client Local Events
		AClientGameEventManager::GetInstance(GetWorld())->OnClientUnitDropped.AddDynamic(this, &AUnitGrid::OnClientUnitDropped);
		AClientGameEventManager::GetInstance(GetWorld())->OnClientUnitDragging.AddDynamic(this, &AUnitGrid::OnClientUnitDragging);
	}
}

void AUnitGrid::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUnitGrid, TopGridCells);
	DOREPLIFETIME(AUnitGrid, BottomGridCells);
}

// Called every frame
void AUnitGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUnitGrid::InitializeGrid()
{
	if (!GridCellClass) return;
	BottomGridCells.SetNum(Rows * Columns);
	TopGridCells.SetNum(Rows * Columns);
	
	for (int32 Row = 0; Row < Rows; ++Row)
	{
		for (int32 Column = 0; Column < Columns; ++Column)
		{
			FVector Location = GetGridCellLocation(Row, Column);
			AUnitCell* NewBottomCell = GetWorld()->SpawnActor<AUnitCell>(GridCellClass, Location, FRotator::ZeroRotator);
			if (NewBottomCell)
			{
				NewBottomCell->InitializeCell(Location, Row, Column);
				BottomGridCells[Row * Columns + Column] = NewBottomCell;
			}
			Location.Y = -Location.Y;
			AUnitCell* NewTopCell = GetWorld()->SpawnActor<AUnitCell>(GridCellClass, Location, FRotator::ZeroRotator);
			if (NewTopCell)
			{
				NewTopCell->InitializeCell(Location, Row, Column);
				TopGridCells[Row * Columns + Column] = NewTopCell;
			}
		}
	}
	
	// TODO: configurable
	//HideRandomCells(2);
}

FVector AUnitGrid::GetGridCellLocation(int32 Row, int32 Column)
{
	return StartLocation + FVector(Column * (CellOffset.X + CellSize.X), Row * (CellOffset.Y + CellSize.Y), 0.0f);
}

AUnitCell* AUnitGrid::GetNearestCell()
{
	return LastHighlightedCell;
}

FVector AUnitGrid::GetNearestCellLocation(const FVector& WorldPosition)
{
	if (LastHighlightedCell)
	{
		return LastHighlightedCell->GetCellCenterLocation();
	}
	FVector LocalPosition = WorldPosition - StartLocation;
	int32 CellX = FMath::RoundToInt(LocalPosition.X / CellSize.X);
	int32 CellY = FMath::RoundToInt(LocalPosition.Y / CellSize.Y);
	int32 CellZ = FMath::RoundToInt(LocalPosition.Z / CellSize.Z);
	
	return StartLocation + FVector(CellX * CellSize.X, CellY * CellSize.Y, CellZ * CellSize.Z);
}

void AUnitGrid::HighlightNearestCell(const FVector& WorldPosition)
{
	AUnitCell* NearestCell = nullptr;
	float MinDistance = FLT_MAX;

	for (int32 Row = 0; Row < Rows; ++Row)
	{
		for (int32 Column = 0; Column < Columns; ++Column)
		{
			AUnitCell* Cell = BottomGridCells[Row * Columns + Column];
			if (Cell && !Cell->IsHidden())
			{
				float Distance = FVector::Dist(WorldPosition, Cell->GetCellCenterLocation());
				if (Distance < MinDistance)
				{
					MinDistance = Distance;
					NearestCell = Cell;
				}
			}
		}
	}
	
	if (NearestCell)
	{
		if (LastHighlightedCell && LastHighlightedCell != NearestCell)
		{
			LastHighlightedCell->HighlightCell(false);
			NearestCell->HighlightCell(true);
		}

		LastHighlightedCell = NearestCell;
	}
}

bool AUnitGrid::IsCellOccupied(AUnitCell* UnitCell) const
{
	return OccupiedCells.Contains(UnitCell) || UnitCell->IsHidden();
}

void AUnitGrid::OccupyCell(AUnitCell* UnitCell, ABaseUnit* Unit)
{
	OccupiedCells.Add(UnitCell, Unit);
}

void AUnitGrid::VacateCell(AUnitCell* UnitCell)
{
	OccupiedCells.Remove(UnitCell);
}

ABaseUnit* AUnitGrid::GetUnitInCell(AUnitCell* Cell)
{
	return *OccupiedCells.Find(Cell);
}

void AUnitGrid::HideRandomCells(int32 num)
{
	// Calculate total cells (assuming a 2D grid)
	int32 TotalCells = Rows * Columns;

	// Set to track generated random numbers
	TSet<int32> GeneratedRandomNum;

	for (int32 i = 0; i < num; ++i)
	{
		int32 randomIndex;
		do 
		{
			// Generate a unique random index
			randomIndex = UKismetMathLibrary::RandomIntegerInRange(0, TotalCells - 1);
		}
		while (GeneratedRandomNum.Contains(randomIndex));  // Ensure it hasn't been used before

		// Add the unique index to the set
		GeneratedRandomNum.Add(randomIndex);

		// Convert the 1D random index to a 2D grid position (row, column)
		int32 Row = randomIndex / Columns;
		int32 Column = randomIndex % Columns;

		// Hide the corresponding cell in the BottomGridCells
		if (BottomGridCells.IsValidIndex(Row * Column + Column))
		{
			BottomGridCells[Row * Column + Column]->SetActorHiddenInGame(true);
		}

		// Optionally hide the corresponding cell in TopGridCells (mirrored position)
		if (TopGridCells.IsValidIndex(Row * Column + Column))
		{
			TopGridCells[Row * Column + Column]->SetActorHiddenInGame(true);
		}
	}
}

void AUnitGrid::OnUnitRemovedFromField(ABaseUnit* Unit)
{
	AUnitCell* CurCell = Unit->GetCurrentCell();
	if (CurCell)
	{
		CurCell->SelectCell(false);
		CurCell->HighlightCell(false);
		VacateCell(CurCell);
		if (LastHighlightedCell)
		{
			LastHighlightedCell->HighlightCell(false);
		}
	}
}

void AUnitGrid::PlaceUnitOnCellLocally(ABaseUnit* BaseUnit)
{
	// TODO
	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, TEXT("PlaceUnitOnCellLocally"));
	if (BaseUnit == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, TEXT("BaseUnit is nullptr"));
		return;
	}
	// Find Old Cell and Vacate it
	AUnitCell* OldCell = BaseUnit->GetCurrentCell();
	if (OldCell)
	{
		this->VacateCell(OldCell);
		OldCell->SelectCell(false);
	}
	//FVector SnappedPosition = UnitGrid->GetNearestCellLocation(GetActorLocation());
	if (AUnitCell* NearestCell = this->GetNearestCell())
	{
		FVector SnappedPosition = NearestCell->GetCellCenterLocation();
		BaseUnit->SetActorLocation(SnappedPosition + BaseUnit->GetOffsetWhenPlace());
		BaseUnit->SetUnitState(EUnitState::WaitingForPlacement);
		// If NearestCell is occupied, we need to switch it with the old cell
		if (this->IsCellOccupied(NearestCell))
		{
			// Switch from a cell to another cell
			if (OldCell)
			{
				ABaseUnit* NearestCellCurUnit = this->GetUnitInCell(NearestCell);
				if (NearestCellCurUnit)
				{
					NearestCellCurUnit->SetActorLocation(OldCell->GetCellCenterLocation() + BaseUnit->GetOffsetWhenPlace());
					this->OccupyCell(OldCell, NearestCellCurUnit);
					NearestCellCurUnit->SetCurrentCell(OldCell);
				}
			}
			else
			{
				// Switch from widget to a unit cell
				ABaseUnit* NearestCellCurUnit = this->GetUnitInCell(NearestCell);
				if (NearestCellCurUnit)
				{
					NearestCellCurUnit->RemoveFromField();
				}
			}
		}
		// Set Nearest Cell occupied
		this->OccupyCell(NearestCell, BaseUnit);
		BaseUnit->SetCurrentCell(NearestCell);
		NearestCell->HighlightCell(false);
	}
}

void AUnitGrid::OnClientUnitDropped(ABaseUnit* BaseUnit, FVector2D UnitLocation)
{
	PlaceUnitOnCellLocally(BaseUnit);
}

void AUnitGrid::OnClientUnitDragging(ABaseUnit* BaseUnit)
{
	if (BaseUnit)
	{
		HighlightNearestCell(BaseUnit->GetActorLocation());
	}
}
