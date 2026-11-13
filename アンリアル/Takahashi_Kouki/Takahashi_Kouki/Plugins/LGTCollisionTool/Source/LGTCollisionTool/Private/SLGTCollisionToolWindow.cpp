#include "SLGTCollisionToolWindow.h"

#include "Editor.h"
#include "LGTCollisionBuilder.h"
#include "Styling/CoreStyle.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "SLGTCollisionToolWindow"

void SLGTCollisionToolWindow::Construct(const FArguments& InArgs)
{
    ChildSlot
    [
        SNew(SBorder)
        .Padding(12.0f)
        [
            SNew(SVerticalBox)

            + SVerticalBox::Slot()
            .AutoHeight()
            [
                SNew(STextBlock)
                .Text(LOCTEXT("Title", "LGT 木 Collision 焼き付け"))
                .Font(FCoreStyle::GetDefaultFontStyle(TEXT("Bold"), 16))
            ]

            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(0.0f, 6.0f, 0.0f, 10.0f)
            [
                SNew(STextBlock)
                .AutoWrapText(true)
                .Text(LOCTEXT(
                    "Description",
                    "Landscape Grass Tool が生成する全インスタンスを同期生成し、同じ Static Mesh を使う見えない HISM として現在のレベルへ保存します。"))
            ]

            + SVerticalBox::Slot()
            .AutoHeight()
            [
                SNew(SCheckBox)
                .IsChecked(this, &SLGTCollisionToolWindow::GetExcludeNoCollisionState)
                .OnCheckStateChanged(this, &SLGTCollisionToolWindow::OnExcludeNoCollisionChanged)
                [
                    SNew(STextBlock)
                    .Text(LOCTEXT("ExcludeNoCollision", "Collision のないメッシュを除外する"))
                ]
            ]

            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(0.0f, 4.0f)
            [
                SNew(SCheckBox)
                .IsChecked(this, &SLGTCollisionToolWindow::GetTreatComplexAsSimpleState)
                .OnCheckStateChanged(this, &SLGTCollisionToolWindow::OnTreatComplexAsSimpleChanged)
                [
                    SNew(STextBlock)
                    .Text(LOCTEXT("ComplexAsSimple", "Complex As Simple を Collision ありとして扱う"))
                ]
            ]

            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(0.0f, 0.0f, 0.0f, 8.0f)
            [
                SNew(SCheckBox)
                .IsChecked(this, &SLGTCollisionToolWindow::GetQueryAndPhysicsState)
                .OnCheckStateChanged(this, &SLGTCollisionToolWindow::OnQueryAndPhysicsChanged)
                [
                    SNew(STextBlock)
                    .Text(LOCTEXT("QueryAndPhysics", "Query And Physics（OFFなら Query Only）"))
                ]
            ]

            + SVerticalBox::Slot()
            .AutoHeight()
            [
                SNew(SHorizontalBox)

                + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(0.0f, 0.0f, 6.0f, 0.0f)
                [
                    SNew(SButton)
                    .Text(LOCTEXT("ScanButton", "調査"))
                    .ToolTipText(LOCTEXT("ScanTooltip", "LGT のメッシュ種類、本数、Collision 状態を調べます。"))
                    .OnClicked(this, &SLGTCollisionToolWindow::OnScanClicked)
                ]

                + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(0.0f, 0.0f, 6.0f, 0.0f)
                [
                    SNew(SButton)
                    .Text(LOCTEXT("RebuildButton", "再構築"))
                    .ToolTipText(LOCTEXT("RebuildTooltip", "古い生成 Actor を削除し、Collision 専用 HISM を作り直します。"))
                    .OnClicked(this, &SLGTCollisionToolWindow::OnRebuildClicked)
                ]

                + SHorizontalBox::Slot()
                .AutoWidth()
                [
                    SNew(SButton)
                    .Text(LOCTEXT("DeleteButton", "生成物を削除"))
                    .ToolTipText(LOCTEXT("DeleteTooltip", "このツールが生成した Actor を削除します。"))
                    .OnClicked(this, &SLGTCollisionToolWindow::OnDeleteClicked)
                ]
            ]

            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(0.0f, 10.0f)
            [
                SNew(SSeparator)
            ]

            + SVerticalBox::Slot()
            .FillHeight(1.0f)
            [
                SNew(SBox)
                .MinDesiredHeight(280.0f)
                [
                    SAssignNew(LogTextBox, SMultiLineEditableTextBox)
                    .IsReadOnly(true)
                    .AutoWrapText(false)
                    .Text(LOCTEXT("InitialLog", "対象レベルを開き、［調査］を押してください。"))
                ]
            ]
        ]
    ];
}

FReply SLGTCollisionToolWindow::OnScanClicked()
{
    UWorld* World = GetEditorWorld();
    if (!World)
    {
        SetLog(TEXT("Editor World を取得できませんでした。"));
        return FReply::Handled();
    }

    SetLog(TEXT("LGT を全域同期生成して調査しています..."));
    const FLGTScanReport Report = FLGTCollisionBuilder::Scan(World, true);
    SetLog(Report.ToText());
    return FReply::Handled();
}

FReply SLGTCollisionToolWindow::OnRebuildClicked()
{
    UWorld* World = GetEditorWorld();
    if (!World)
    {
        SetLog(TEXT("Editor World を取得できませんでした。"));
        return FReply::Handled();
    }

    FLGTBuildOptions Options;
    Options.bExcludeMeshesWithoutCollision = bExcludeMeshesWithoutCollision;
    Options.bTreatComplexAsSimpleAsCollision = bTreatComplexAsSimpleAsCollision;
    Options.bUseQueryAndPhysics = bUseQueryAndPhysics;

    SetLog(TEXT("Collision 専用 HISM を再構築しています..."));
    const FLGTBuildReport Report = FLGTCollisionBuilder::Rebuild(World, Options);
    SetLog(Report.Message);
    return FReply::Handled();
}

FReply SLGTCollisionToolWindow::OnDeleteClicked()
{
    UWorld* World = GetEditorWorld();
    if (!World)
    {
        SetLog(TEXT("Editor World を取得できませんでした。"));
        return FReply::Handled();
    }

    const int32 DeletedCount = FLGTCollisionBuilder::DeleteGeneratedActors(World);
    SetLog(FString::Printf(TEXT("生成 Actor を %d 個削除しました。レベルを保存してください。"), DeletedCount));
    return FReply::Handled();
}

ECheckBoxState SLGTCollisionToolWindow::GetExcludeNoCollisionState() const
{
    return bExcludeMeshesWithoutCollision ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SLGTCollisionToolWindow::OnExcludeNoCollisionChanged(ECheckBoxState NewState)
{
    bExcludeMeshesWithoutCollision = NewState == ECheckBoxState::Checked;
}

ECheckBoxState SLGTCollisionToolWindow::GetTreatComplexAsSimpleState() const
{
    return bTreatComplexAsSimpleAsCollision ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SLGTCollisionToolWindow::OnTreatComplexAsSimpleChanged(ECheckBoxState NewState)
{
    bTreatComplexAsSimpleAsCollision = NewState == ECheckBoxState::Checked;
}

ECheckBoxState SLGTCollisionToolWindow::GetQueryAndPhysicsState() const
{
    return bUseQueryAndPhysics ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SLGTCollisionToolWindow::OnQueryAndPhysicsChanged(ECheckBoxState NewState)
{
    bUseQueryAndPhysics = NewState == ECheckBoxState::Checked;
}

void SLGTCollisionToolWindow::SetLog(const FString& Text)
{
    if (LogTextBox.IsValid())
    {
        LogTextBox->SetText(FText::FromString(Text));
    }
}

UWorld* SLGTCollisionToolWindow::GetEditorWorld() const
{
    return GEditor ? GEditor->GetEditorWorldContext().World() : nullptr;
}

#undef LOCTEXT_NAMESPACE
