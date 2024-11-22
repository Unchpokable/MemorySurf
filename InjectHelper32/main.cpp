#include "procutils.h"
#include "arguments.h"

int main(int argc, char** argv) {
    auto args = ParseArguments(argc, argv);

    if(args.mode == Inject) {
        return InjectProcess(args);
    }

    if (args.mode == Unload) {
        return UnloadProcess(args);
    }
}
