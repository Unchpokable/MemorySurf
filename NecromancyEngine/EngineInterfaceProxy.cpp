#include "pch.h"
#include "EngineInterfaceProxy.h"

EngineInterface* EngineInterfaceProxy::getEngine() {
    return _engine;
}

void EngineInterfaceProxy::setEngine(EngineInterface* engine) {
    _engine = engine;
}
