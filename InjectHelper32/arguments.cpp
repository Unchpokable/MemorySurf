#include "arguments.h"
#include <string>
#include <stdexcept>
#include <sstream>

InjectorArguments ParseArguments(int argc, char** argv)
{
    if(argc != 4) {
        std::stringstream err;
        err << "Should be given 3 cl args but " << argc << " given. ";
        err << "Usage: -i/-u <procId> <dllPath>";
        throw std::invalid_argument(err.str());
    }

    InjectorArguments args;

    args.mode = static_cast<InjectorMode>(*reinterpret_cast<uint8_t*>(argv[1] + 1)); // +1 cause we need to skip `-` character

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
