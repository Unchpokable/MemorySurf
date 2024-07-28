#pragma once

class EngineInterfaceProxy {
public:
    static const EngineInterface* getEngine();
    static void setEngine(EngineInterface* engine);

private:
    static EngineInterface* _engine { nullptr };
};

