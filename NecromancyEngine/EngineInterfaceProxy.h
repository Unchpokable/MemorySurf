#pragma once

class EngineInterfaceProxy {
public:
    static EngineInterface* getEngine();
    static void setEngine(EngineInterface* engine);

private:
    inline static EngineInterface* _engine = nullptr;
};

