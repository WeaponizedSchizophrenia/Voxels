#pragma once

#include "Global.hpp"
#include "OctTreeHelpers.hpp"
#include "ext/vector_float3.hpp"
#include "ext/vector_int3.hpp"
#include <array>
#include <functional>
#include <memory>

namespace voxels {
    template<typename T>
    class OctTreeNode {
    public:
        explicit OctTreeNode(uint32 depth, glm::vec3 origin) noexcept
            : depth(depth)
            , origin(origin) {  }
        virtual ~OctTreeNode() = default;

        virtual void forEach(std::function<void(T&)> callback) = 0;
        virtual void enumerate(std::function<void(T&, uint32)> callback) = 0;
        virtual void insert(T&& data, glm::ivec3 index) = 0;

        const uint32 depth;
        const glm::vec3 origin;
    };

    template<typename T>
    class OctTreeLeaf: public OctTreeNode<T> {
    public:
        using OctTreeNode<T>::depth;

        OctTreeLeaf(T&& data, uint32 depth, glm::vec3 origin) noexcept : data(std::move(data)), OctTreeNode<T>(depth, origin) {  }
        virtual ~OctTreeLeaf() override = default;

        virtual void forEach(std::function<void(T&)> callback) override {
            callback(data);
        }

        virtual void enumerate(std::function<void(T&, uint32)> callback) override {
            callback(data, depth);
        }

        virtual void insert(T&& data, glm::ivec3) override {
            data = std::move(data);
        }

        [[nodiscard]] inline const T& getData() const noexcept { return data; }
        [[nodiscard]] inline T& getData() noexcept { return data; }

    private:
        T data;
    };

    template<typename T>
    class OctTreeBranch: public OctTreeNode<T> {
    public:
        using OctTreeNode<T>::depth, OctTreeNode<T>::origin;
        using ChildNode = std::unique_ptr<OctTreeNode<T>>;

        OctTreeBranch() noexcept = default;
        OctTreeBranch(std::array<ChildNode, 8>&& children, uint32 depth, glm::vec3 origin) noexcept
            : m_children(std::move(children))
            , OctTreeNode<T>(depth, origin) {  }

        virtual ~OctTreeBranch() override = default;

        virtual void forEach(std::function<void(T&)> callback) override {
            for(auto& child : m_children) {
                if(child)
                    child->forEach(callback);
            }
        }
        virtual void enumerate(std::function<void(T&, uint32)> callback) override {
            for(auto& child : m_children) {
                if(child)
                    child->enumerate(callback);
            }
        }

        virtual void insert(T&& data, glm::ivec3 index) override {
            auto sideLength = 1 << (MAX_OCT_TREE_DEPTH - depth);
            auto sIndex = voxels::convertToScalarIndex(voxels::convertToNormalizedIndex(index, sideLength));
            if(m_children[sIndex]) {
                m_children[sIndex]->insert(std::move(data), index);
            }
        }

    private:
        std::array<ChildNode, 8> m_children;
    };
}