// Global needs to be included because it defines what window API is being used.
#include "Global.hpp"
#include "Application.hpp"

int main() {
    auto application = voxels::Application();

    application.start();

    return 0;
}
