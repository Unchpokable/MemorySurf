#include "arguments.h"
#include <string>
#include <stdexcept>

InjectorArguments ParseArguments(int argc, char** argv) {
    if(argc != 4) {
        throw std::invalid_argument("Should be given 3 cl args. Usage: -i/-u <procId> <dllPath>");
    }

    InjectorArguments args;

    args.mode = static_cast<InjectorMode>(*reinterpret_cast<uint16_t*>(argv[1]));

    try {
        args.procId = std::stoul(argv[2]);
    } catch(const std::exception&) {
        throw std::invalid_argument("Invalid process ID. It should be a numeric value.");
    }

    std::string dllPathStr = argv[3];
    if(dllPathStr.length() >= MAX_PATH) {
        throw std::invalid_argument("DLL path exceeds the maximum allowed length.");
    }

    if(MultiByteToWideChar(CP_UTF8, 0, dllPathStr.c_str(), -1, args.dllPath, MAX_PATH) == 0) {
        throw std::runtime_error("Failed to convert DLL path to wide string.");
    }

    return args;
}

