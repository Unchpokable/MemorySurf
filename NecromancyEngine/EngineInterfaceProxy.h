#pragma once

class EngineInterfaceProxy {
public:
    static EngineInterface* getEngine();
    static void setEngine(EngineInterface* engine);

private:
    static EngineInterface* _engine { nullptr };
};

