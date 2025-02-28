#pragma once

#include "thirdparty/imgui/imgui.h"
#include "thirdparty/imgui/backends/imgui_impl_dx11.h"
#include "thirdparty/imgui/backends/imgui_impl_win32.h"

class IUIComponent
{
public:
    virtual ~IUIComponent();

    virtual void update() = 0;
    virtual void render() = 0;
};
