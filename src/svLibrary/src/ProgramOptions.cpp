#include <cstring>

#include <sv/ProgramOptions.h>

namespace sv {
int32_t ProgramOptions::checkOption(const std::string &option) const {
    int32_t result = -1;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], option.c_str()) == 0) {
            result = i;
            break;
        }
    }

    return result;
}

const char *ProgramOptions::getOption(size_t index) const {
    const char *option = nullptr;

    if (index < argc) {
        option = argv[index];
    }

    return option;
}
}
