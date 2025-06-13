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
	void InitializeCell(FVector Location, int32 Row, int32 Column, bool IsSpecial);

	UFUNCTION(BlueprintCallable, Category = "GridCell")
	void HighlightCell(bool bHighlight);

	UFUNCTION(BlueprintCallable, Category = "GridCell")
	void SelectCell(bool bSelected);

	FVector GetCellCenterLocation();

	bool GetIsSpecial() const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* CellMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cell")
	UMaterialInterface* DefaultMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cell")
	UMaterialInterface* HighlightMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cell")
	UMaterialInterface* SelectMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cell")
	UMaterialInterface* SpecialSelectMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cell")
	UMaterialInterface* SpecialMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cell")
	UMaterialInterface* SpecialHighlightMaterial;

	int32 CellRow;
	int32 CellColumn;

	UPROPERTY()
	bool bIsSelected;

	UPROPERTY(ReplicatedUsing = OnRep_bIsSpecial)
	bool bIsSpecial;

	UFUNCTION()
	void OnRep_bIsSpecial();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
