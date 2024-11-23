#include <exception>
#include <iostream>

#include "procutils.h"
#include "arguments.h"

int main(int argc, char** argv) {
    try
    {
        auto args = ParseArguments(argc, argv);

        if(args.mode == Inject) {
            return InjectProcess(args);
        }

        if (args.mode == Unload) {
            return UnloadProcess(args);
        }
    } catch (std::exception &ex) {
        std::cerr << ex.what();
    }
}
