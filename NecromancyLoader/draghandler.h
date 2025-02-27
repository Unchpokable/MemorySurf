#pragma once

#include <QObject>

class DragHandler : public QObject
{
    Q_OBJECT

public:
    DragHandler(QObject* parent);
    virtual ~DragHandler() override = default;

protected:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;

private:
    QWidget* _parentWindow;
    QPoint _dragStartPosition;
    bool _isDragging = false;
};
