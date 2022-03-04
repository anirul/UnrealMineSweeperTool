#pragma once

#include <functional>
#include <map>
#include "CoreMinimal.h"

/** 
 * Mine Sweeper Logic
 * 
 * This is the place where the game rules are set and applied, this part is 
 * mainly in STL C++ as This doesn't deal with UE in any way shape or form.
 * It could be rewritten in pure UE C++ like using TMap and other UE utility
 * but as this was not specified in the document I prefer to write in STL than
 * in UE (also I don't know if there is a way to shuffle a TArray?).
 */
class MineSweeperLogic
{
public:
    // A slot is a place on the Mine Sweeper map.
    struct SlotValue
    {
        // This are the value that the place could have.
        // I also store the number of neighbor to have a kicker access.
        enum class SlotEnum : uint8
        {
            UNKNOWN = 0,    // Unknown?
            LABEL_1 = 1,    // 1 neighbor
            LABEL_2 = 2,    // 2
            LABEL_3 = 3,    // 3
            LABEL_4 = 4,    // 4
            LABEL_5 = 5,    // 5
            LABEL_6 = 6,    // 6
            LABEL_7 = 7,    // 7
            LABEL_8 = 8,    // 8
            EMPTY = 9,      // Nothing in it!
            MINE = 10,      // Mine there is a mine here!
            FLAG = 11,      // Flag (not implemented)
        };
        // Value of the Slot (default to unknown).
        SlotEnum EnumValue = SlotEnum::UNKNOWN;
        // Revealed the place was discovered (clicked).
        bool Discovered = false;
    };

public:
    // Constructor take a size by default.
    MineSweeperLogic(int32 dx, int32 dy) : _dx(dx), _dy(dy) {}
    // Initialize the position of mines and the number on the map.
    bool InitializeGrid(int32 mineCount);
    // Discover a given slot (mark is as Discovered).
    void DiscoverSlot(int32 x, int32 y);
    // Get the value at position (doesn't change anything).
    SlotValue operator()(int32 x, int32 y) const;
    // Check if we succeeded (win the game).
    bool CheckSuccess() const;
    // Check if we exploded (lost the game).
    bool CheckExploded() const;
    // Reveal all (in case of success or failure).
    void RevealAll();

private:
    // Return the number of mine around yourself (8).
    int CountMineAround(int32 x, int32 y);
    // Execute a function at all location around (8).
    void CallAllAround(
        int32 x, 
        int32 y,
        std::function<void(int32, int32)> call);

private:
    // Size of the map.
    int32 _dx;
    int32 _dy;
    // Slot values this is stored as a map as this is quite easy to check if a
    // value is present or not in a map and the performance is not that slower
    // O(log(n)) vs O(1).
    std::map<std::pair<int32, int32>, SlotValue> _slotValues;
};