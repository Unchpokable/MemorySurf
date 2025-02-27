#pragma once

namespace necromancy::messages {
enum class MessageIds : uint16_t
{
    ShortDump = 0x0001,
    FullDump  = 0x0002
};

enum class FieldType : uint16_t
{
    Integer      = 0x0003,
    Float        = 0x0004,
    IntegerArray = 0x0005,
    FloatArray   = 0x0006
};
}
