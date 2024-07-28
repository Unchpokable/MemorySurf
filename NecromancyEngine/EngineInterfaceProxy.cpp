#include "pch.h"
#include "EngineInterfaceProxy.h"

const EngineInterface * EngineInterfaceProxy::getEngine() {
    return _engine;
}

void EngineInterfaceProxy::setEngine(EngineInterface* engine) {
    _engine = engine;
}
