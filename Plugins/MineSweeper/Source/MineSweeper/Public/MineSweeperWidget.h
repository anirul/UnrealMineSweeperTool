// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"
#include "MineSweeperLogic.h"

/**
 * Widget for mine sweeper app.
 * 
 * This contains the main window controls for the mine sweeper app and the
 * Display of the mine sweeper part. It also include the mine sweeper logic
 * that contain the logic for the game.
 */
class SMineSweeperWidget : public SCompoundWidget
{
public:
    // No construction parameters.
    SLATE_BEGIN_ARGS(SMineSweeperWidget) {}
    SLATE_END_ARGS()

public:
    // Build the main app (buttons and grid).
    void Construct(const FArguments& args);
    // Accessors to the main parameters for the grid and game.
    TOptional<int32> GetGridWidth() const { return _widthValue; }
    TOptional<int32> GetGridHeight() const { return _heightValue; }
    TOptional<int32> GetMineCount() const { return _mineCount; }
    void SetGridWidth(int32 value) { _widthValue = value; }
    void SetGridHeight(int32 value) { _heightValue = value; }
    void SetMineCount(int32 value) { _mineCount = value; }
    // The callback for the button that recreate the game grid.
    FReply OnGenerate();
    // This callback is for each button on the map.
    FReply OnButtonClick(int32 x, int32 y);

private:
    // Update the grid size in case you change the parameter or you want a new
    // seed.
    void UpdateGridSize();
    // Redraw the grid this is called once after each click.
    void UpdateGridLayout();
    // Check if you win or loose the game.
    void CheckVictory();
    // This transfer from enum to TCHAR used to display what is on slot in the
    // grid, I had a direct transfer but due to TCHAR it didn't work properly.
    TCHAR GetCharFromEnum(
        MineSweeperLogic::SlotValue::SlotEnum enumValue) const;

protected:
    // Pointer to the game logic.
    TUniquePtr<MineSweeperLogic> _mineSweeperLogic;
    // Pointer to the grid panel (the GUI Part).
    TSharedPtr<SUniformGridPanel> _uniformGridPanel;
    // Default values.
    int32 _widthValue = 8;
    int32 _heightValue = 4;
    int32 _mineCount = 5;
};
