#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SMultiLineEditableTextBox;

class SLGTCollisionToolWindow final : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SLGTCollisionToolWindow) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:
    FReply OnScanClicked();
    FReply OnRebuildClicked();
    FReply OnDeleteClicked();

    ECheckBoxState GetExcludeNoCollisionState() const;
    void OnExcludeNoCollisionChanged(ECheckBoxState NewState);

    ECheckBoxState GetTreatComplexAsSimpleState() const;
    void OnTreatComplexAsSimpleChanged(ECheckBoxState NewState);

    ECheckBoxState GetQueryAndPhysicsState() const;
    void OnQueryAndPhysicsChanged(ECheckBoxState NewState);

    void SetLog(const FString& Text);
    UWorld* GetEditorWorld() const;

    bool bExcludeMeshesWithoutCollision = true;
    bool bTreatComplexAsSimpleAsCollision = true;
    bool bUseQueryAndPhysics = true;

    TSharedPtr<SMultiLineEditableTextBox> LogTextBox;
};
