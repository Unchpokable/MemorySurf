#include "layout.h"

Layout::Layout()
{
    _orientation = Orientation::Horizontal;
    _verticalAlignment = VerticalAlignment::Top;
    _horizontalAlignment = HorizontalAlignment::Left;
    _box = Box::empty();
}

Layout::~Layout()
{
    for(auto component : _components) {
        delete component;
    }
}

void Layout::render(ImVec2& cursorPosition)
{
    switch(_orientation) {
        case Orientation::Horizontal:
            return renderHorizontal(cursorPosition);
        case Orientation::Vertical:
            return renderVertical(cursorPosition);
    }
}

void Layout::addComponent(IUIComponent* component)
{
    if(component != nullptr) {
        _components.push_back(component);
    }

    updateWorkingSize();
}

void Layout::setOrientation(Orientation orientation)
{
    _orientation = orientation;
}

Orientation Layout::getOrientation() const
{
    return _orientation;
}

void Layout::setVerticalAlignment(VerticalAlignment alignment)
{
    _verticalAlignment = alignment;
}

VerticalAlignment Layout::getVerticalAlignment() const
{
    return _verticalAlignment;
}

void Layout::setHorizontalAlignment(HorizontalAlignment alignment)
{
    _horizontalAlignment = alignment;
}

HorizontalAlignment Layout::getHorizontalAlignment() const
{
    return _horizontalAlignment;
}

void Layout::renderHorizontal(ImVec2& cursorPosition)
{
    // todo
}

void Layout::renderVertical(ImVec2& cursorPosition)
{
    // todo
}

void Layout::updateWorkingSize()
{
    float maxY = std::numeric_limits<float>::min();
    float accX = 0;

    ImVec2 size { accX, maxY };

    for(auto component : _components) {
        auto& box = component->boxSize().size;
        if(box.y > size.y) {
            size.y = box.y;
        }

        size.x += box.x;
        size.x += ImGui::GetStyle().ItemSpacing.x;
    }

    size.x -= ImGui::GetStyle().ItemSpacing.x;

    _box.size = size;
}
