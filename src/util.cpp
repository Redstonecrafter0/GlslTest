#include "util.h"
#include <iostream>

std::filesystem::path checkPath(std::filesystem::path path) {
    if (!exists(path)) {
        std::cerr << "error: path " << std::filesystem::weakly_canonical(path) << " not found" << std::endl;
        exit(-1);
    }
    return path;
}
