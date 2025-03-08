#pragma once

#include <vector>

#include "alignment.h"
#include "iuicomponent.h"

class Layout : public IUIComponent
{
public:
    explicit Layout();
    virtual ~Layout() override;

    virtual void render(ImVec2& cursorPosition) override;

    void addComponent(IUIComponent* component);

    void setOrientation(Orientation orientation);
    Orientation getOrientation() const;

    void setVerticalAlignment(VerticalAlignment alignment);
    VerticalAlignment getVerticalAlignment() const;

    void setHorizontalAlignment(HorizontalAlignment alignment);
    HorizontalAlignment getHorizontalAlignment() const;

protected:
    void renderHorizontal(ImVec2& cursorPosition);
    void renderVertical(ImVec2& cursorPosition);

private:
    void updateWorkingSize();

    std::vector<IUIComponent*> _components;
    Orientation _orientation;
    VerticalAlignment _verticalAlignment;
    HorizontalAlignment _horizontalAlignment;
};
