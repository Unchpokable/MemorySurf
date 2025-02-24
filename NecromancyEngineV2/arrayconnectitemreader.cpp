#include "pch.h"

#include "arrayconnectitemreader.h"

#include "genericutils.h"

necromancy::Memory::ArrayConnectItemReader::ArrayConnectItemReader(void* arrayConnectItem) : _functions(hooks::HkFunctions::setup()) {
    _arrayConnectItem = arrayConnectItem;
}

void* necromancy::Memory::ArrayConnectItemReader::getColumn() const {
    auto func = _functions.get<Typedefs::ArrayConnectItem_GetColumn>("ArrayConnectItem_GetColumn");
    return func(_arrayConnectItem);
}

int necromancy::Memory::ArrayConnectItemReader::getRowCount() const {
    auto func = _functions.get<Typedefs::ArrayConnectItem_GetRowCount>("ArrayConnectItem_GetRowCount");
    return func(_arrayConnectItem);
}

void* necromancy::Memory::ArrayConnectItemReader::getRow(int idx) const {
    auto func = _functions.get<Typedefs::ArrayConnectItem_GetRow>("ArrayConnectItem_GetRow");
    return func(_arrayConnectItem, idx);
}
