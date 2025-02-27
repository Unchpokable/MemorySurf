#include "pch.h"
#include "draghandler.h"

DragHandler::DragHandler(QObject* parent)
: QObject(parent), _parentWindow(qobject_cast<QWidget*>(parent))
{
    // empty
}

bool DragHandler::eventFilter(QObject* watched, QEvent* event)
{
    if(!_parentWindow) {
        return false;
    }

    if(event->type() == QEvent::MouseButtonPress) {
        auto* mouseEvent = static_cast<QMouseEvent*>(event);
        if(mouseEvent->button() == Qt::LeftButton) {
            _dragStartPosition = mouseEvent->globalPosition().toPoint();
            _isDragging = true;
            return true;
        }
    }
    else if(event->type() == QEvent::MouseMove) {
        auto* mouseEvent = static_cast<QMouseEvent*>(event);
        if(_isDragging && (mouseEvent->buttons() & Qt::LeftButton)) {
            const QPoint delta = mouseEvent->globalPosition().toPoint() - _dragStartPosition;
            _parentWindow->move(_parentWindow->pos() + delta);
            _dragStartPosition = mouseEvent->globalPosition().toPoint();
            return true;
        }
    }
    else if(event->type() == QEvent::MouseButtonRelease) {
        auto* mouseEvent = static_cast<QMouseEvent*>(event);
        if(mouseEvent->button() == Qt::LeftButton) {
            _isDragging = false;
            return true;
        }
    }

    return QObject::eventFilter(watched, event);
}
