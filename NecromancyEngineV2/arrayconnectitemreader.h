#pragma once

#include "vtabledef.h"
#include "hkfunctions.h"

namespace necromancy::Memory {

class ArrayConnectItemReader final {
public:
    ArrayConnectItemReader(void* arrayConnectItem);
    ~ArrayConnectItemReader() = default;

    void* getRow(int idx) const;
    void* getColumn() const;
    int getRowCount() const;

private:
    hooks::HkFunctions _functions;
    void* _arrayConnectItem;
};

}

