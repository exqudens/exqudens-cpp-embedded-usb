#include <cstdio>

#include "exqudens/embedded/usb/hal/HardwareUtils.hpp"
#include "exqudens/embedded/usb/versions.hpp"

namespace exqudens {

    std::array<char, 32> HardwareUtils::getVersion() {
        std::array<char, 32> result = {};
        (void) std::snprintf(result.data(), result.size(), "%u.%u.%u", PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH);
        return result;
    }

}
