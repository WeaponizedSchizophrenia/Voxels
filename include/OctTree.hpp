#pragma once

#include "OctTreeNode.hpp"

namespace voxels {

    template<typename T>
    class OctTree {
    public:
        explicit OctTree(T&& data, glm::vec3 center) noexcept
            : m_root(OctTreeBranch<T>(std::move(data), 0, center)) {  }

        [[nodiscard]] inline OctTreeBranch<T>& getRoot() noexcept { return m_root; }
        [[nodiscard]] inline const OctTreeBranch<T>& getRoot() const noexcept { return m_root; }

    private:
        OctTreeBranch<T> m_root;
    };
}