#pragma once

#define AUTO_PROPERTY(Type, name)          \
private:                                   \
    Type _##name;                          \
public:                                    \
    Type name() const { return _##name; }  \
    void set_##name(const Type& value) { _##name = value; }
