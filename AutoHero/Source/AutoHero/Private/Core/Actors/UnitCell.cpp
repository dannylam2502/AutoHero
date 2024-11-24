// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Actors/UnitCell.h"


// Sets default values
AUnitCell::AUnitCell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	CellMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CellMesh"));
	RootComponent = CellMesh;
}

// Called when the game starts or when spawned
void AUnitCell::BeginPlay()
{
	Super::BeginPlay();
	// Set the default material
    if (DefaultMaterial)
    {
        CellMesh->SetMaterial(0, DefaultMaterial);
    }
}

void AUnitCell::InitializeCell(FVector Location, int32 Row, int32 Column)
{
	SetActorLocation(Location);
	CellRow = Row;
	CellColumn = Column;
}

void AUnitCell::HighlightCell(bool bHighlight)
{
	if (bIsSelected)
	{
		return;
	}
	if (bHighlight && HighlightMaterial)
	{
		CellMesh->SetMaterial(0, HighlightMaterial);
	}
	else if (DefaultMaterial)
	{
		CellMesh->SetMaterial(0, DefaultMaterial);
	}
}

void AUnitCell::SelectCell(bool bSelected)
{
	if (bIsSelected != bSelected)
	{
		if (bSelected && SelectMaterial)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("SelectCELL True"));
			CellMesh->SetMaterial(0, SelectMaterial);
		}
		else if (DefaultMaterial)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("SelectCELL FALSE"));
			CellMesh->SetMaterial(0, DefaultMaterial);
		}
		bIsSelected = bSelected;
	}
}

FVector AUnitCell::GetCellCenterLocation()
{
	return GetActorLocation();
}

// Called every frame
void AUnitCell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

