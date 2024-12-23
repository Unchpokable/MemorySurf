#pragma once

#include "globaldef.h"
#include "hkfunctions.h"

namespace Necromancy::Memory {

class ArrayConnectItemReader final {
public:
    ArrayConnectItemReader(void* arrayConnectItem);
    ~ArrayConnectItemReader() = default;

    void* getRow(int idx) const;
    void* getColumn() const;
    int getRowCount() const;

private:
    Detours::HkFunctions _functions;
    void* _arrayConnectItem;
};

}

