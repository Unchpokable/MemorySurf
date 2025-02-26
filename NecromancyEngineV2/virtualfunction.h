#pragma once

template<typename T>
struct function_traits;

template<typename R, typename... Args>
struct function_traits<R(Args...)> {
    using return_type = R;
};

template<typename R, typename... Args>
struct function_traits<R(*)(Args...)> : function_traits<R(Args...)> {};

template<typename R, typename... Args>
struct function_traits<R(__thiscall*)(Args...)> : function_traits<R(Args...)> {};

template<typename CFunc>
class VirtualFunction final {
public:
    using ReturnType = typename function_traits<CFunc>::return_type;

    VirtualFunction(CFunc ptr) : _ptr(ptr) {}
    VirtualFunction(void* ptr) : _ptr(reinterpret_cast<CFunc>(ptr)) {}

    // call w\o arguments
    ReturnType operator()(void* _this) {
        return _ptr(_this);
    }

    // call with arguments
    template<typename... Args>
    ReturnType operator()(void* _this, Args... args) {
        return _ptr(_this, args...);
    }

    CFunc ptr() {
        return _ptr;
    }

private:
    CFunc _ptr;
};
