#include <exception>
#include <iostream>

#include "procutils.h"
#include "arguments.h"

int main(int argc, char** argv)
{
    try {
        auto args = ParseArguments(argc, argv);

        if(args.mode == Inject) {
            std::cerr << InjectProcess(args);
        }

        if(args.mode == Unload) {
            std::cerr << UnloadProcess(args);
        }
    } catch(std::exception& ex) {
        std::cerr << ex.what();
    }

    return 0;
}
