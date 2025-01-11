#pragma once

#include "hkfunctions.h"
#include "typedefs.h"
#include "vftableutils.hpp"
#include "vtabledef.h"

namespace Necromancy::Memory::Internals {

static constexpr ptrdiff_t ArrayTable_GetElementsVTableIdx = VTABLE_INDEX(8);
static constexpr ptrdiff_t ArrayTable_GetElementAtVTableIdx = VTABLE_INDEX(4);

template<typename Channel = A3d_Channel>
class TableLayout final {
public:
    using ItemType = std::conditional_t<std::is_same_v<Channel, void*>, Channel, Channel*>;
    static_assert(std::is_pointer_v<ItemType>);

    TableLayout(A3d_Channel* target);
    ~TableLayout() = default;

    ItemType getItem(std::int32_t row, std::int32_t col) const;
    std::int32_t getTotalSize() const;

    std::vector<std::int32_t> getBounding() const;

private:
    void* getAcoColumn(std::int32_t idx) const;

    void* _arrayTable;
    Detours::HkFunctions _functions;
};

template<typename Channel>
TableLayout<Channel>::TableLayout(A3d_Channel *target) : _arrayTable(target), _functions(Detours::HkFunctions::setup()) {
    // empty
}

template<typename Channel>
typename TableLayout<Channel>::ItemType TableLayout<Channel>::getItem(std::int32_t row, std::int32_t col) const {
    auto columnSizeFunc = _functions.get<Typedefs::ArrayConnectItem_GetRowCount>("ArrayConnectItem_GetRowCount");

    auto column = getAcoColumn(col);

    auto columnSize = columnSizeFunc(column);

    if(columnSize > row) {
        return nullptr;
    }

    auto getRowItemFunc = _functions.get<Typedefs::ArrayConnectItem_GetRow>("ArrayConnectItem_GetRow");

    // ReSharper disable once CppReinterpretCastFromVoidPtr
    return reinterpret_cast<ItemType>(getRowItemFunc(column, row));
}

template<typename Channel>
std::int32_t TableLayout<Channel>::getTotalSize() const {
    auto bounds = getBounding();
    return std::accumulate(bounds.begin(), bounds.end(), 0);
}

template<typename Channel>
std::vector<std::int32_t> TableLayout<Channel>::getBounding() const {
    std::vector<std::int32_t> sizes;

    auto getRowCountFunc = _functions.get<Typedefs::ArrayConnectItem_GetRowCount>("ArrayConnectItem_GetRowCount");

    auto columnCount = Memory::CallVTable<std::int32_t, Typedefs::ArrayTable_GetElementCount>(_arrayTable, ArrayTable_GetElementsVTableIdx, _arrayTable);

    sizes.reserve(columnCount);
    for(auto i { 0 }; i < columnCount; i++) {
        auto column = Memory::CallVTable<void*, Typedefs::ArrayTable_GetElementAtIndex>(_arrayTable, ArrayTable_GetElementAtVTableIdx, _arrayTable, i);
        auto rowCount = getRowCountFunc(column);
        sizes.push_back(rowCount);
    }

    return sizes;
}

template<typename Channel>
void* TableLayout<Channel>::getAcoColumn(std::int32_t idx) const {
    auto columnCount = Memory::CallVTable<std::int32_t, Typedefs::ArrayTable_GetElementCount>(_arrayTable, ArrayTable_GetElementsVTableIdx, _arrayTable);
    if(idx > columnCount || idx < 0) {
        return nullptr;
    }

    return Memory::CallVTable<void*, Typedefs::ArrayTable_GetElementAtIndex>(_arrayTable, ArrayTable_GetElementAtVTableIdx, _arrayTable, idx);
}

}

