// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnitCell.generated.h"

UCLASS()
class AUTOHERO_API AUnitCell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUnitCell();

	UFUNCTION(BlueprintCallable, Category = "GridCell")
	void InitializeCell(FVector Location, int32 Row, int32 Column);

	UFUNCTION(BlueprintCallable, Category = "GridCell")
	void HighlightCell(bool bHighlight);

	FVector GetCellCenterLocation();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* CellMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cell")
	UMaterialInterface* DefaultMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cell")
	UMaterialInterface* HighlightMaterial;

	int32 CellRow;
	int32 CellColumn;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
