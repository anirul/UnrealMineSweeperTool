#include "MineSweeperLogic.h"
#include <algorithm>
#include <random>

bool MineSweeperLogic::InitializeGrid(int32 mineCount)
{
    if (mineCount > _dx * _dy) return false;
    _slotValues.clear();
    std::vector<std::pair<int32, int32>> positions;
    for (auto i = 0; i < _dx; ++i)
    {
        for (auto j = 0; j < _dy; ++j)
        {
            _slotValues.insert({ 
                {i, j}, 
                { SlotValue::SlotEnum::EMPTY, false } });
            positions.push_back({ i, j });
        }
    }
    static auto rnd = std::default_random_engine{};
    std::shuffle(positions.begin(), positions.end(), rnd);
    for (int i = 0; i < mineCount; ++i)
    {
        _slotValues.at(positions[i]).EnumValue = SlotValue::SlotEnum::MINE;
    }
    for (auto& position : _slotValues)
    {
        if (position.second.EnumValue == SlotValue::SlotEnum::EMPTY)
        {
            int count = CountMineAround(
                position.first.first, 
                position.first.second);
            position.second.EnumValue =
                (count == 0) ?
                    SlotValue::SlotEnum::EMPTY :
                    static_cast<SlotValue::SlotEnum>(count);
        }
    }
    return true;
}

void MineSweeperLogic::DiscoverSlot(int32 x, int32 y)
{
    auto it = _slotValues.find({ x, y });
    if (it == _slotValues.end()) return;
    if (it->second.Discovered) return;
    it->second = { it->second.EnumValue, true };
    if (it->second.EnumValue == SlotValue::SlotEnum::EMPTY)
    {
        CallAllAround(x, y, [this](int32 x, int32 y) { DiscoverSlot(x, y); });
    }
}

MineSweeperLogic::SlotValue MineSweeperLogic::operator()(int32 x, int32 y) const
{
    auto it = _slotValues.find({ x, y });
    if (it == _slotValues.end()) return {};
    return it->second;
}

bool MineSweeperLogic::CheckSuccess() const
{
    for (const auto& item : _slotValues)
    {
        if (item.second.EnumValue == SlotValue::SlotEnum::MINE)
        {
            if (item.second.Discovered) return false;
        }
        else
        {
            if (!item.second.Discovered) return false;
        }
    }
    return true;
}

bool MineSweeperLogic::CheckExploded() const
{
    for (const auto& item : _slotValues)
    {
        if ((item.second.EnumValue == SlotValue::SlotEnum::MINE) &&
            item.second.Discovered)
        {
            return true;
        }
    }
    return false;
}

void MineSweeperLogic::RevealAll()
{
    for (auto& item : _slotValues)
    {
        item.second.Discovered = true;
    }
}

int MineSweeperLogic::CountMineAround(int32 x, int32 y)
{
    int count = 0;
    CallAllAround(x, y, [this, &count](int32 x, int32 y) 
        {
            auto it = _slotValues.find({ x, y });
            if (it == _slotValues.end()) return;
            if (it->second.EnumValue == SlotValue::SlotEnum::MINE) count++;
        });
    return count;
}

void MineSweeperLogic::CallAllAround(
    int32 x,
    int32 y,
    std::function<void(int32, int32)> call)
{
    for (auto i = x - 1; i < x + 2; ++i)
    {
        for (auto j = y - 1; j < y + 2; ++j)
        {
            if (i == x && j == y) continue;
            call(i, j);
        }
    }
}
