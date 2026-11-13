#pragma once

#include "CoreMinimal.h"

class UStaticMesh;
class UWorld;

struct FLGTMeshScanResult
{
    TWeakObjectPtr<UStaticMesh> Mesh;
    int32 InstanceCount = 0;

    bool bHasSimpleCollision = false;
    bool bUsesComplexAsSimple = false;

    // Static Mesh アセット側の Collision 設定
    bool bAssetCollisionEnabled = false;
    FName CollisionProfileName = NAME_None;
};

struct FLGTScanReport
{
    int32 LandscapeCount = 0;
    int32 SourceComponentCount = 0;
    int64 TotalInstanceCount = 0;
    TArray<FLGTMeshScanResult> Meshes;
    TArray<FString> Warnings;

    FString ToText() const;
};

struct FLGTBuildOptions
{
    bool bExcludeMeshesWithoutCollision = true;
    bool bTreatComplexAsSimpleAsCollision = true;
    bool bUseQueryAndPhysics = true;
};

struct FLGTBuildReport
{
    bool bSucceeded = false;
    int32 GeneratedComponentCount = 0;
    int64 GeneratedInstanceCount = 0;
    int32 SkippedMeshCount = 0;
    int64 SkippedInstanceCount = 0;
    FString Message;
};

class FLGTCollisionBuilder
{
public:
    static FLGTScanReport Scan(UWorld* World, bool bForcePopulateGrass = true);
    static FLGTBuildReport Rebuild(UWorld* World, const FLGTBuildOptions& Options);
    static int32 DeleteGeneratedActors(UWorld* World);

    static const FName GeneratedActorTag;
    static const TCHAR* GeneratedActorLabel;
};
