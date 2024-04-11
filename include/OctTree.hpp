#pragma once

#include "OctTreeNode.hpp"

namespace voxels {

    template<typename T>
    class OctTree {
    public:
        explicit OctTree(T&& data, glm::vec3 center) noexcept
            : m_root(OctTreeLeaf<T>(std::move(data), 0, center)) {  }
    private:
        OctTreeNode<T> m_root;
    };
}