#pragma once

#include "q3dchannelreader.h"
#include "q3dfloatreader.h"
#include "tablelayoutpresenter.h"

namespace Necromancy::Memory {

template<ValidPrimitiveReader InternalReader = Q3DFloatReader>
class Q3DArrayTableReader final : Q3DChannelReader
{
    using InternalReaderData = typename InternalReader::DataType;
    using TypedVector = std::vector<InternalReaderData>;

public:
    Q3DArrayTableReader(A3d_Channel* target);
    virtual ~Q3DArrayTableReader() override = default;

    int getElementsCount() const;
    std::vector<void*> getElements() const;

    TypedVector getElementsTyped() const;

private:
    void* getArrayTable() const;
    
    Internals::TableLayout<void*> _table;
};

template<ValidPrimitiveReader InternalReader>
Q3DArrayTableReader<InternalReader>::Q3DArrayTableReader(A3d_Channel* target) : Q3DChannelReader(target), _table(reinterpret_cast<A3d_Channel*>(getArrayTable())) {
    // empty
}

template<ValidPrimitiveReader InternalReader>
int Q3DArrayTableReader<InternalReader>::getElementsCount() const {
    return _table.getTotalSize();
}

///@brief extracts and returns ArrayTable elements using Row-Major table layout. Even if Quest3D Uses Col-Major notation, this function will invert it to Row-Major because Row-Major is more human
template<ValidPrimitiveReader InternalReader>
std::vector<void*> Q3DArrayTableReader<InternalReader>::getElements() const {
    std::vector<void*> data {};

    auto bounding = _table.getBounding(); // std::vector<int32_t> of lengths of every column
    
    std::vector<std::vector<void*>> colwiseData;

    for(std::size_t i { 0 }; i < bounding.size(); i++) {
        auto columnSize = bounding[i];
        for(std::ptrdiff_t j { 0 }; j < columnSize; j++) {
            colwiseData[i][j] = _table.getItem(j, i);
        }
    }

    std::size_t maxRows = 0;
    for(const auto& column : colwiseData) {
        if(column.size() > maxRows) {
            maxRows = column.size();
        }
    }

    data.reserve(maxRows * colwiseData.size()); 

    for(std::size_t row = 0; row < maxRows; ++row) {
        for(std::size_t col = 0; col < colwiseData.size(); ++col) {
            if(row < colwiseData[col].size()) {
                data.push_back(colwiseData[col][row]);
            } else {
                data.push_back(nullptr);
            }
        }
    }

    return data;
}

template<ValidPrimitiveReader InternalReader>
typename Q3DArrayTableReader<InternalReader>::TypedVector Q3DArrayTableReader<InternalReader>::getElementsTyped() const {
    TypedVector result;
    auto rawData = getElements();
    for(auto item : rawData) {
        InternalReader reader(reinterpret_cast<A3d_Channel*>(item));
        result.push_back(reader.get());
    }

    return result;
}

template<ValidPrimitiveReader InternalReader>
void* Q3DArrayTableReader<InternalReader>::getArrayTable() const {
    return _functions.get<Aco_ArrayTable_GetTable>("Aco_ArrayTable_GetTable")(_target);
}

}

