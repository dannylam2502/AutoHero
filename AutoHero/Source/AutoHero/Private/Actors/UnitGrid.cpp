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
	NumRows = 5;
	NumCols = 4;
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
	//if (HasAuthority() && !IsRunningDedicatedServer())
	if (HasAuthority())
	{
		// Server logic
		InitializeGrid();
	}

	ENetMode NetMode = GetNetMode();
	if (NetMode == NM_Standalone || NetMode == NM_Client)
	{
		// Register client events, even in Standalone
		AClientGameEventManager::GetInstance(GetWorld())->OnClientUnitDropped.AddDynamic(this, &AUnitGrid::OnClientUnitDropped);
		AClientGameEventManager::GetInstance(GetWorld())->OnClientUnitDragging.AddDynamic(this, &AUnitGrid::OnClientUnitDragging);
	
	}
}

void AUnitGrid::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	AClientGameEventManager* Manager = AClientGameEventManager::GetInstance(GetWorld());
	if (Manager)
	{
		Manager->OnClientUnitDropped.RemoveDynamic(this, &AUnitGrid::OnClientUnitDropped);
		Manager->OnClientUnitDragging.RemoveDynamic(this, &AUnitGrid::OnClientUnitDragging);
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
	BottomGridCells.SetNum(NumRows * NumCols);
	TopGridCells.SetNum(NumRows * NumCols);

	SpecialCellIndex = FMath::RandRange(0, NumRows * NumCols - 1);
	
	for (int32 Row = 0; Row < NumRows; ++Row)
	{
		for (int32 Column = 0; Column < NumCols; ++Column)
		{
			FVector Location = GetGridCellLocation(Row, Column);
			AUnitCell* NewBottomCell = GetWorld()->SpawnActor<AUnitCell>(GridCellClass, Location, FRotator::ZeroRotator);
			if (NewBottomCell)
			{
				// Is Special Index?
				bool IsSpecial = false;
				if (Row * NumCols + Column == SpecialCellIndex)
				{
					IsSpecial = true;
				}
				NewBottomCell->InitializeCell(Location, Row, Column, IsSpecial);
				BottomGridCells[Row * NumCols + Column] = NewBottomCell;
			}
			Location.Y = -Location.Y;
			AUnitCell* NewTopCell = GetWorld()->SpawnActor<AUnitCell>(GridCellClass, Location, FRotator::ZeroRotator);
			if (NewTopCell)
			{
				int32 TopMirrorCol = NumCols - 1 - Column;
				bool bIsSpecial = false;
				if (Row * NumCols + TopMirrorCol == SpecialCellIndex)
				{
					bIsSpecial = true;
				}
				NewTopCell->InitializeCell(Location, Row, Column, bIsSpecial);
				TopGridCells[Row * NumCols + TopMirrorCol] = NewTopCell;
			}
		}
	}
	
	// TODO: configurable
	HideRandomCells(2);
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

void AUnitGrid::HighlightNearestCell(EActorTeam Team, const FVector& WorldPosition)
{
	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Blue,
		FString::Printf(TEXT("Team is %hs"), Team == EActorTeam::Blue ? "Blue" : "Red"));
	TArray<AUnitCell*> UnitCellsList = (Team == EActorTeam::Blue ? BottomGridCells : TopGridCells);
	AUnitCell* NearestCell = nullptr;
	float MinDistance = FLT_MAX;

	for (int32 Row = 0; Row < NumRows; ++Row)
	{
		for (int32 Column = 0; Column < NumCols; ++Column)
		{
			AUnitCell* Cell = UnitCellsList[Row * NumCols + Column];
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
	int32 TotalCells = NumRows * NumCols;

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
		// Ensure it hasn't been used before and not a special index
		while (randomIndex == SpecialCellIndex || GeneratedRandomNum.Contains(randomIndex));  

		// Add the unique index to the set
		GeneratedRandomNum.Add(randomIndex);

		// Convert the 1D random index to a 2D grid position (row, column)
		int32 Row = randomIndex / NumCols;
		int32 Column = randomIndex % NumCols;

		// Hide the corresponding cell in the BottomGridCells
		if (BottomGridCells.IsValidIndex(Row * NumCols + Column))
		{
			BottomGridCells[Row * NumCols + Column]->SetActorHiddenInGame(true);
		}

		int32 TopColumn = NumCols - 1 - Column;
		// Optionally hide the corresponding cell in TopGridCells (mirrored position)
		if (TopGridCells.IsValidIndex(Row * NumCols + TopColumn))
		{
			TopGridCells[Row * NumCols + TopColumn]->SetActorHiddenInGame(true);
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
		AClientGameEventManager::GetInstance(GetWorld())->BroadCastVacateCellEvent(Unit);
		if (LastHighlightedCell)
		{
			LastHighlightedCell->HighlightCell(false);
		}
	}
}

void AUnitGrid::OnClientUnitDropped(ABaseUnit* BaseUnit, FVector2D UnitLocation)
{
	PlaceUnitOnCellLocally(BaseUnit);
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
		AClientGameEventManager::GetInstance(GetWorld())->BroadCastVacateCellEvent(BaseUnit);
	}
	//FVector SnappedPosition = UnitGrid->GetNearestCellLocation(GetActorLocation());
	if (AUnitCell* NearestCell = this->GetNearestCell())
	{
		FVector SnappedPosition = NearestCell->GetCellCenterLocation();
		FVector Offset = BaseUnit->GetOffsetWhenPlace();
		FVector FinalPosition = SnappedPosition + Offset;

		UE_LOG(LogTemp, Warning, TEXT("Unit [%s] - SnappedPosition: %s | Offset: %s | FinalPosition: %s"),
			*BaseUnit->GetName(),
			*SnappedPosition.ToString(),
			*Offset.ToString(),
			*FinalPosition.ToString()
		);

		BaseUnit->SetActorLocation(FinalPosition);
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
				// Exchange the position of Unit from widget to a unit cell
				ABaseUnit* NearestCellCurUnit = this->GetUnitInCell(NearestCell);
				if (NearestCellCurUnit)
				{
					NearestCellCurUnit->RemoveFromField();
					OnUnitRemovedFromField(NearestCellCurUnit);
				}
			}
		}
		// Set Nearest Cell occupied
		this->OccupyCell(NearestCell, BaseUnit);
		BaseUnit->SetCurrentCell(NearestCell);
		NearestCell->HighlightCell(false);
		AClientGameEventManager::GetInstance(GetWorld())->BroadCastUnitOccupiedEvent(BaseUnit, FVector2D());
	}
}

AUnitCell* AUnitGrid::GetCellByPosition(EActorTeam InTeam, int32 Row, int32 Col)
{
	if (Row < 0 || Row >= NumRows || Col < 0 || Col >= NumCols)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid grid position (%d, %d)"), Row, Col);
		return nullptr;
	}

	int32 Index = Row * NumCols + Col;
	if (InTeam == EActorTeam::Blue)
	{
		return BottomGridCells.IsValidIndex(Index) ? BottomGridCells[Index] : nullptr;
	}
	return TopGridCells.IsValidIndex(Index) ? TopGridCells[Index] : nullptr;
}

TArray<AUnitCell*> AUnitGrid::GetCellsInRow(EActorTeam InTeam, int32 RowIndex)
{
	TArray<AUnitCell*> RowCells;

	if (RowIndex < 0 || RowIndex >= NumRows)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid row index: %d"), RowIndex);
		return RowCells;
	}

	for (int32 Col = 0; Col < NumCols; ++Col)
	{
		AUnitCell* Cell = GetCellByPosition(InTeam, RowIndex, Col);
		if (Cell)
		{
			RowCells.Add(Cell);
		}
	}

	return RowCells;
}

void AUnitGrid::OnClientUnitDragging(ABaseUnit* BaseUnit)
{
	if (BaseUnit)
	{
		GEngine->AddOnScreenDebugMessage(4, 5.0f, FColor::Red,
			FString::Printf(TEXT("OnClientUnitDragging %hs"), BaseUnit->ETeam == EActorTeam::Red ? "Red" : "Blue"));
		HighlightNearestCell(BaseUnit->ETeam, BaseUnit->GetActorLocation());
	}
}
