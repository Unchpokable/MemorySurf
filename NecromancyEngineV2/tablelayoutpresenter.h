#pragma once

#include "hkfunctions.h"
#include "typedefs.h"

namespace Necromancy::Memory::Internals {

template<typename Channel = A3d_Channel>
class TableLayout final {
public:
    using ItemType = std::conditional_t<std::is_same_v<Channel, void*>, Channel, Channel*>;
    static_assert(std::is_pointer_v<ItemType>);

    TableLayout(A3d_Channel* target);
    ~TableLayout() = default;

    ItemType getItem(int row, int col) const;
    int getTotalSize() const;

    std::vector<int> getBounding() const;

private:
    void* getAcoColumn(int idx) const;

    void* _arrayTable;
    Detours::HkFunctions _functions;
};

template<typename Channel>
TableLayout<Channel>::TableLayout(A3d_Channel *target) : _arrayTable(target), _functions(Detours::HkFunctions::setup()) {
    // empty
}

template<typename Channel>
typename TableLayout<Channel>::ItemType TableLayout<Channel>::getItem(int row, int col) const {
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
int TableLayout<Channel>::getTotalSize() const {
    auto bounds = getBounding();
    return std::accumulate(bounds.begin(), bounds.end(), 0);
}

template<typename Channel>
std::vector<int> TableLayout<Channel>::getBounding() const {
    std::vector<int> sizes;

    auto getColCountFunc = _functions.get<Typedefs::ArrayTable_GetElementCount>("ArrayTable_GetElementCount");
    auto getColumnFunc = _functions.get<Typedefs::ArrayTable_GetElementAtIndex>("ArrayTable_GetElementAtIndex");
    auto getRowCountFunc = _functions.get<Typedefs::ArrayConnectItem_GetRowCount>("ArrayConnectItem_GetRowCount");

    auto columnCount = getColCountFunc(_arrayTable);

    for(auto i { 0 }; i < columnCount; i++) {
        auto column = getColumnFunc(_arrayTable, i);
        auto rowCount = getRowCountFunc(column);
        sizes[i] = rowCount;
    }

    return sizes;
}

template<typename Channel>
void* TableLayout<Channel>::getAcoColumn(int idx) const {
    auto getColCountFunc = _functions.get<Typedefs::ArrayTable_GetElementCount>("ArrayTable_GetElementCount");
    auto getColumnFunc = _functions.get<Typedefs::ArrayTable_GetElementAtIndex>("ArrayTable_GetElementAtIndex");

    auto columnCount = getColCountFunc(_arrayTable);
    if(idx > columnCount || idx < 0) {
        return nullptr;
    }

    return getColumnFunc(_arrayTable, idx);
}

}

