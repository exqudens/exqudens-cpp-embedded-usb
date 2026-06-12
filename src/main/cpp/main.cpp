#include "exqudens/embedded/usb/Application.hpp"

int main() {
    int32_t result = exqudens::Application::run();
    while (1) {}
    return result;
}
