#pragma once

#include "Global.hpp"
#include "OctTreeHelpers.hpp"
#include "ext/vector_float3.hpp"
#include <array>
#include <functional>
#include <memory>

namespace voxels {
    template<typename T>
    class OctTreeNode {
    public:
        explicit OctTreeNode(uint32 depth, glm::vec3 center) noexcept
            : depth(depth)
            , center(center) {  }
        virtual ~OctTreeNode() = default;

        virtual void forEach(std::function<void(OctTreeNode<T>&)> callback) = 0;
        virtual void insert(T&& data, glm::vec3 position) = 0;
        [[nodiscard]] virtual bool equal(const OctTreeNode<T>& other) const noexcept = 0;

        [[nodiscard]] virtual uint32 getLength() const noexcept {
            return 1 << (MAX_OCT_TREE_DEPTH - depth);
        }

        const uint32 depth;
        const glm::vec3 center;
    };

    template<typename T>
    class OctTreeLeaf: public OctTreeNode<T> {
    public:
        using OctTreeNode<T>::depth;

        OctTreeLeaf(T&& data, uint32 depth, glm::vec3 center) noexcept : data(std::move(data)), OctTreeNode<T>(depth, center) {  }
        virtual ~OctTreeLeaf() override = default;

        virtual void forEach(std::function<void(OctTreeNode<T>&)> callback) override {
            callback(*this);
        }

        virtual void insert(T&& data, glm::vec3) override {
            data = std::move(data);
        }

        [[nodiscard]] virtual bool equal(const OctTreeNode<T>& other) const noexcept override {
            if(depth == other.depth) {
                if(auto leaf = dynamic_cast<const OctTreeLeaf<T>*>(&other)) {
                    return leaf->data == data;
                }
            }
            return false;
        }

        [[nodiscard]] inline const T& getData() const noexcept { return data; }
        [[nodiscard]] inline T& getData() noexcept { return data; }

    private:
        T data;
    };

    template<typename T>
    class OctTreeBranch: public OctTreeNode<T> {
    public:
        using OctTreeNode<T>::depth, OctTreeNode<T>::center, OctTreeNode<T>::getLength;
        using ChildNode = std::unique_ptr<OctTreeNode<T>>;

        OctTreeBranch() noexcept = delete;
        explicit OctTreeBranch(std::array<ChildNode, 8>&& children, uint32 depth, glm::vec3 center) noexcept
            : m_children(std::move(children))
            , OctTreeNode<T>(depth, center) {  }
        explicit OctTreeBranch(T& data, uint32 depth, glm::vec3 center) noexcept 
            : voxels::OctTreeNode<T>(depth, center)
            , m_children([&] -> std::array<ChildNode, 8> {
                // The half of the childs length is the quarter of this length.
                float half = getLength() / 4;
                return {
                    std::make_unique<OctTreeLeaf<T>>(std::move(data), depth + 1, center + glm::vec3{ -half, half, -half }),
                    std::make_unique<OctTreeLeaf<T>>(std::move(data), depth + 1, center + glm::vec3{ -half, half, half }),
                    std::make_unique<OctTreeLeaf<T>>(std::move(data), depth + 1, center + glm::vec3{ -half, -half, -half }),
                    std::make_unique<OctTreeLeaf<T>>(std::move(data), depth + 1, center + glm::vec3{ -half, -half, half }),
                    std::make_unique<OctTreeLeaf<T>>(std::move(data), depth + 1, center + glm::vec3{ half, half, -half }),
                    std::make_unique<OctTreeLeaf<T>>(std::move(data), depth + 1, center + glm::vec3{ half, half, half }),
                    std::make_unique<OctTreeLeaf<T>>(std::move(data), depth + 1, center + glm::vec3{ half, -half, -half }),
                    std::make_unique<OctTreeLeaf<T>>(std::move(data), depth + 1, center + glm::vec3{ half, -half, half }),
                };
            }()) {  }

        virtual ~OctTreeBranch() override = default;

        virtual void forEach(std::function<void(OctTreeNode<T>&)> callback) override {
            for(auto& child : m_children) {
                if(child)
                    child->forEach(callback);
            }
        }

        virtual void insert(T&& data, glm::vec3 position) override {

            auto delta = position - center;
            auto nIndex = convertToNormalizedIndex(delta);
            auto index = convertToScalarIndex(nIndex);

            if(depth == MAX_OCT_TREE_DEPTH - 1) {
                auto newCenter = center + glm::vec3{
                    static_cast<int>(nIndex.x) * (getLength() / 2),
                    static_cast<int>(nIndex.y) * (getLength() / 2),
                    static_cast<int>(nIndex.z) * (getLength() / 2)  
                };
                m_children[index] = std::make_unique<OctTreeLeaf<T>>(std::move(data), depth + 1, newCenter);
                return;
            }

            if(auto branch = dynamic_cast<OctTreeBranch<T>*>(m_children[index].get())) {
                branch->insert(std::move(data), position);
            } else if(auto leaf = dynamic_cast<OctTreeLeaf<T>*>(m_children[index].get())) {
                auto leafCenter = leaf->center;
                auto delta = position - leafCenter;
                auto nIndex = convertToNormalizedIndex(delta);
                auto index = convertToScalarIndex(nIndex);
                m_children[index] = std::make_unique<OctTreeBranch<T>>(leaf->getData(), leaf->depth, leafCenter);
                m_children[index]->insert(std::move(data), position);
            }
        }

        [[nodiscard]] virtual bool equal(const OctTreeNode<T>& other) const noexcept override {
            if(depth == other.depth) {
                if(auto branch = dynamic_cast<const OctTreeBranch<T>*>(&other)) {
                    if(childrenAreEqual() && branch->childrenAreEqual() && m_children.front()->equal(*branch->m_children.front())) {
                        return true;
                    }
                }
            }
            return false;
        }

        [[nodiscard]] bool childrenAreEqual() const noexcept {
            for(int i = 1; i < 8; ++i) {
                if(!m_children[0]->equal(*m_children[i]))
                    return false;
            }
            return true;
        }

    private:
        std::array<ChildNode, 8> m_children;
    };
}