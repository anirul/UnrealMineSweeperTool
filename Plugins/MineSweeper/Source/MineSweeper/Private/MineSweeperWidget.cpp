// Fill out your copyright notice in the Description page of Project Settings.


#include "MineSweeperWidget.h"

void SMineSweeperWidget::Construct(const FArguments& args)
{
    ChildSlot
    [
        SNew(SVerticalBox)
        + SVerticalBox::Slot()
            .HAlign(HAlign_Fill)
            .FillHeight(0.25)
            [
                SNew(SVerticalBox)
                + SVerticalBox::Slot()
                    .HAlign(HAlign_Fill)
                    .VAlign(VAlign_Top)
                    [
                        SNew(SHorizontalBox)
                        + SHorizontalBox::Slot()
                            .HAlign(HAlign_Center)
                            .VAlign(VAlign_Center)
                            [
                                SNew(STextBlock)
                                .Text(FText::FromString(TEXT("Width:")))
                            ]
                        + SHorizontalBox::Slot()
                            .HAlign(HAlign_Center)
                            .VAlign(VAlign_Center)
                            [
                                SNew(SNumericEntryBox<int32>)
                                    .AllowSpin(true)
                                    .MinValue(1)
                                    .MaxValue(32)
                                    .MinSliderValue(1)
                                    .MaxSliderValue(32)
                                    .Value(
                                        this, 
                                        &SMineSweeperWidget::GetGridWidth)
                                    .OnValueChanged(
                                        this, 
                                        &SMineSweeperWidget::SetGridWidth)
                            ]
                    ]
                + SVerticalBox::Slot()
                    .HAlign(HAlign_Fill)
                    .VAlign(VAlign_Top)
                    [
                        SNew(SHorizontalBox)
                        + SHorizontalBox::Slot()
                        .HAlign(HAlign_Center)
                        .VAlign(VAlign_Center)
                        [
                            SNew(STextBlock)
                            .Text(FText::FromString(TEXT("Height:")))
                        ]
                        + SHorizontalBox::Slot()
                            .HAlign(HAlign_Center)
                            .VAlign(VAlign_Center)
                            [
                                SNew(SNumericEntryBox<int32>)
                                .AllowSpin(true)
                                .MinValue(1)
                                .MaxValue(32)
                                .MinSliderValue(1)
                                .MaxSliderValue(32)
                                .Value(
                                    this,
                                    &SMineSweeperWidget::GetGridHeight)
                                .OnValueChanged(
                                    this,
                                    &SMineSweeperWidget::SetGridHeight)
                            ]
                    ]
                + SVerticalBox::Slot()
                    .HAlign(HAlign_Fill)
                    .VAlign(VAlign_Top)
                    [
                        SNew(SHorizontalBox)
                        + SHorizontalBox::Slot()
                        .HAlign(HAlign_Center)
                        .VAlign(VAlign_Center)
                        [
                            SNew(STextBlock)
                            .Text(FText::FromString(TEXT("Mines:")))
                        ]
                        + SHorizontalBox::Slot()
                            .HAlign(HAlign_Center)
                            .VAlign(VAlign_Center)
                            [
                                SNew(SNumericEntryBox<int32>)
                                .AllowSpin(true)
                                .MinValue(1)
                                .MaxValue(64)
                                .MinSliderValue(1)
                                .MaxSliderValue(64)
                                .Value(
                                    this,
                                    &SMineSweeperWidget::GetMineCount)
                                .OnValueChanged(
                                    this,
                                    &SMineSweeperWidget::SetMineCount)
                            ]
                    ]
                + SVerticalBox::Slot()
                    .HAlign(HAlign_Center)
                    .VAlign(VAlign_Top)
                    [
                        SNew(SButton)
                            .Text(FText::FromString(TEXT("Generate new grid")))
                            .OnClicked(this, &SMineSweeperWidget::OnGenerate)
                    ]
            ]
        + SVerticalBox::Slot()
            .HAlign(HAlign_Fill)
            .FillHeight(0.75)
            [
                SNew(SBorder)
                [
                        SAssignNew(_uniformGridPanel, SUniformGridPanel)
                ]
            ]
    ];
    OnGenerate();
}

void SMineSweeperWidget::UpdateGridSize()
{
    _mineSweeperLogic = MakeUnique<MineSweeperLogic>(_widthValue, _heightValue);
    // Probably too many bombs.
    while (!_mineSweeperLogic->InitializeGrid(_mineCount))
    {
        FMessageDialog::Open(
            EAppMsgType::Ok,
            FText::FromString(TEXT("Too many bombs? reduced to 1.")));
        _mineCount = 1;
    }
}

void SMineSweeperWidget::CheckVictory()
{
    if (_mineSweeperLogic->CheckSuccess())
    {
        _mineSweeperLogic->RevealAll();
        FMessageDialog::Open(
            EAppMsgType::Ok,
            FText::FromString(TEXT("Success! You found all the bombs!")));
    }
    else if (_mineSweeperLogic->CheckExploded())
    {
        _mineSweeperLogic->RevealAll();
        FMessageDialog::Open(
            EAppMsgType::Ok,
            FText::FromString(TEXT("Failure, you can try again?")));
    }
}

void SMineSweeperWidget::UpdateGridLayout()
{
    
    _uniformGridPanel->ClearChildren();
    for (auto x = 0; x < _widthValue; ++x)
    {
        for (auto y = 0; y < _heightValue; ++y)
        {
            auto slotState = _mineSweeperLogic->operator ()(x, y);
            if (slotState.Discovered == false)
            {
                _uniformGridPanel->AddSlot(x, y)
                    .HAlign(HAlign_Fill)
                    .VAlign(VAlign_Fill)
                    [
                        SNew(SButton)
                            .HAlign(HAlign_Center)
                            .VAlign(VAlign_Center)
                            .Text(FText::FromString(TEXT("?")))
                            .OnClicked_Lambda([this, x, y]() ->FReply
                                {
                                    return OnButtonClick(x, y);
                                })
                    ];
            }
            else
            {
                TCHAR c = GetCharFromEnum(slotState.EnumValue);
                FString stringValue = " ";
                stringValue[0] = c;
                _uniformGridPanel->AddSlot(x, y)
                    .HAlign(HAlign_Fill)
                    .VAlign(VAlign_Fill)
                    [
                        SNew(SBorder)
                            .HAlign(HAlign_Center)
                            .VAlign(VAlign_Center)
                            [
                                SNew(STextBlock)
                                .Text(FText::FromString(stringValue))
                            ]
                    ];
            }
        }
    }
}

FReply SMineSweeperWidget::OnGenerate()
{
    UpdateGridSize();
    UpdateGridLayout();
    return FReply::Handled();
}

FReply SMineSweeperWidget::OnButtonClick(int32 x, int32 y)
{
    if (_mineSweeperLogic)
        _mineSweeperLogic->DiscoverSlot(x, y);
    CheckVictory();
    UpdateGridLayout();
    return FReply::Handled();
}

TCHAR SMineSweeperWidget::GetCharFromEnum(
    MineSweeperLogic::SlotValue::SlotEnum enumValue) const
{
    switch (enumValue)
    {
    case MineSweeperLogic::SlotValue::SlotEnum::EMPTY:
        return ' ';
    case MineSweeperLogic::SlotValue::SlotEnum::FLAG:
        return 'P';
    case MineSweeperLogic::SlotValue::SlotEnum::MINE:
        return 'M';
    case MineSweeperLogic::SlotValue::SlotEnum::LABEL_1:
        return '1';
    case MineSweeperLogic::SlotValue::SlotEnum::LABEL_2:
        return '2';
    case MineSweeperLogic::SlotValue::SlotEnum::LABEL_3:
        return '3';
    case MineSweeperLogic::SlotValue::SlotEnum::LABEL_4:
        return '4';
    case MineSweeperLogic::SlotValue::SlotEnum::LABEL_5:
        return '5';
    case MineSweeperLogic::SlotValue::SlotEnum::LABEL_6:
        return '6';
    case MineSweeperLogic::SlotValue::SlotEnum::LABEL_7:
        return '7';
    case MineSweeperLogic::SlotValue::SlotEnum::LABEL_8:
        return '8';
    default:
    case MineSweeperLogic::SlotValue::SlotEnum::UNKNOWN:
        return 'U';
    }
}
