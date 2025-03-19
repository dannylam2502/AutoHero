// Fill out your copyright notice in the Description page of Project Settings.


#include "Singletons/UnitDataManager.h"

UUnitDataManager* UUnitDataManager::Get()
{
	static UUnitDataManager* Singleton = NewObject<UUnitDataManager>();
	if (Singleton)
	{
		Singleton->AddToRoot();
		Singleton->Initialize();
	}
	return Singleton;
}

void UUnitDataManager::Initialize()
{
	// Load the DataTable asset dynamically
	FString DataTablePath = TEXT("/Game/Data/DT_Unit.DT_Unit");
	UnitDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *DataTablePath));

	if (!UnitDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load DataTable at %s"), *DataTablePath);
	}
	PopulateUnitDataMap();
	// Load the DataTable asset
    // static ConstructorHelpers::FObjectFinder<UDataTable> DataTableObj(TEXT("DataTable'/Game/Data/DT_Unit'"));
    // if (DataTableObj.Succeeded())
    // {
    //     UnitDataTable = DataTableObj.Object;
    // }
}

FUnitData* UUnitDataManager::GetUnitDataByID(int32 UnitID)
{
	FUnitData* UnitData = UnitDataMap.Find(UnitID);
	return UnitData;
}

void UUnitDataManager::PopulateUnitDataMap()
{
	if (!UnitDataTable) return;

	// Clear the map before populating it
	UnitDataMap.Empty();

	// Get all rows from the DataTable
	static const FString ContextString(TEXT("UUnitDataManager"));
	TArray<FUnitData*> AllRows;
	UnitDataTable->GetAllRows<FUnitData>(ContextString, AllRows);

	// Populate the map
	for (FUnitData* Row : AllRows)
	{
		if (Row)
		{
			UnitDataMap.Add(Row->UnitID, *Row);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Populated UnitDataMap with %d entries"), UnitDataMap.Num());

}

