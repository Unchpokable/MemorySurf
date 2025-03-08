#pragma once

#include "thirdparty/imgui/imgui.h"
#include "thirdparty/imgui/backends/imgui_impl_dx11.h"
#include "thirdparty/imgui/backends/imgui_impl_win32.h"

class IUIComponent
{
public:
    struct Box
    {
        ImVec2 position;
        ImVec2 size;

        static Box empty() { return {ImVec2(0, 0), ImVec2(0, 0) }; }
    };

    virtual ~IUIComponent();

    virtual void update() {} // empty by default but not required to implement in any component
    virtual void render(ImVec2& cursorPosition) = 0;

    Box& boxSize() { return _box; }
    const Box& boxSize() const { return _box; }
    void setBoxSize(const Box& boxSize) { _box = boxSize; }

protected:
    Box _box;
};
