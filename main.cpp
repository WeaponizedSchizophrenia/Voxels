// Global needs to be included because it defines what window API is being used.
#include "Global.hpp"
#include "Application.hpp"
#include "OctTreeNode.hpp"
#include <iostream>

int main() {
    std::srand(time(nullptr));
    auto i = std::rand() % 500 + 500;
    auto root = voxels::OctTreeBranch<int>(i, 0u, { 32.0f, 32.0f, 32.0f });

    root.insert(69, {1.0f, 1.0f, 54.0f});
    root.insert(420, { 26, 31, 2 });

    root.forEach([](auto& node) {
        if(auto leaf = dynamic_cast<voxels::OctTreeLeaf<int>*>(&node)) {
            std::cout << '[' << node.depth << "] {" << node.center.x << ", " << node.center.y << ", " << node.center.z;
            std::cout << "} = " << leaf->getData() << '\n';
        }
    });

    auto application = voxels::Application();

    application.start();

    return 0;
}
