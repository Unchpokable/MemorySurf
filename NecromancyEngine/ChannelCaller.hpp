#pragma once
#include <type_traits>
#include "traits.hpp"

template<typename CVal>
class ChannelCaller
{
public:
    template<typename T, std::enable_if_t<is_a3d_channel_v<T>, bool> = true>
    void assignChannel(T* channelPtr);

    void assignGetFunc(CVal(__thiscall* readFunc)(void*));

    void assignGetDefault(CVal(__thiscall* readDefaultFunc)(void*));

    void assignSet(void(__thiscall* setFunc)(void*, CVal));

    CVal get();
    CVal getDefault();
    void set(CVal value);

private:
    CVal(__thiscall* _channelGet)(void* channelSelf) { nullptr };
    CVal(__thiscall* _channelGetDefault)(void* channelSelf) { nullptr };
    void(__thiscall* _channelSet)(void* channelSelf, CVal chValue) {nullptr };

    void* _channelSelf { nullptr };
};

template <typename CVal>
template <typename T, std::enable_if_t<is_a3d_channel_v<T>, bool>>
void ChannelCaller<CVal>::assignChannel(T* channelPtr)
{
    _channelSelf = channelPtr;
}

template<typename CVal>
void ChannelCaller<CVal>::assignGetFunc(CVal(__thiscall *readFunc)(void*)) {
    _channelGet = readFunc;
}

template <typename CVal>
void ChannelCaller<CVal>::assignGetDefault(CVal (__thiscall *readDefaultFunc)(void*))
{
    _channelGetDefault = readDefaultFunc;
}

template <typename CVal>
void ChannelCaller<CVal>::assignSet(void (__thiscall *setFunc)(void*, CVal))
{
    _channelSet = setFunc;
}

template<typename CVal>
CVal ChannelCaller<CVal>::get() {
    return _channelGet(_channelSelf);
}

template<typename CVal>
CVal ChannelCaller<CVal>::getDefault() {
    return _channelGetDefault(_channelSelf);
}

template<typename CVal>
void ChannelCaller<CVal>::set(CVal value) {
    _channelSet(_channelSelf, value);
}
