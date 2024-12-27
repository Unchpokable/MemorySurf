#include "pch.h"

#include "arrayconnectiemreader.h"

#include "genericutils.h"

Necromancy::Memory::ArrayConnectItemReader::ArrayConnectItemReader(void* arrayConnectItem) : _functions(Detours::HkFunctions::setup()) {
    _arrayConnectItem = arrayConnectItem;
}

void* Necromancy::Memory::ArrayConnectItemReader::getColumn() const {
    auto func = _functions.get<Typedefs::ArrayConnectItem_GetColumn>("ArrayConnectItem_GetColumn");
    return func(_arrayConnectItem);
}

int Necromancy::Memory::ArrayConnectItemReader::getRowCount() const {
    auto func = _functions.get<Typedefs::ArrayConnectItem_GetRowCount>("ArrayConnectItem_GetRowCount");
    return func(_arrayConnectItem);
}

void* Necromancy::Memory::ArrayConnectItemReader::getRow(int idx) const {
    auto func = _functions.get<Typedefs::ArrayConnectItem_GetRow>("ArrayConnectItem_GetRow");
    return func(_arrayConnectItem, idx);
}
