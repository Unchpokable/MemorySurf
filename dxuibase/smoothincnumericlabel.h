#pragma once

#include <deque>

// for some reason <tweeny.h> header not fully self-contained and requires some other includes to work. So there it is
#include "includetweeny.h"

#include "iuicomponent.h"

class SmoothIncNumericLabel : public IUIComponent
{
    struct SmoothTextData {
        tweeny::tween<double> progress;
        double initialValue;
        double remainingValue;
    };

public:
    explicit SmoothIncNumericLabel();
    virtual ~SmoothIncNumericLabel() override;

    virtual void update() override;
    virtual void render(ImVec2& cursorPosition) override;

    /// @brief appends a new value to queue that will partition added to final value
    void enqueueAppend(double value);

    /// @brief returns a const pointer to underlying field that stores value of current numeric label
    const double* value();

private:
    double _finalValue;
    std::deque<SmoothTextData> _queuedAppends;
};