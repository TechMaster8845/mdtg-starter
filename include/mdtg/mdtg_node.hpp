#pragma once
#include <vector>
#include <memory>
#include <string>
#include <mutex>
#include "matrix4.hpp"

using PropertyKey = std::string;
using PropertyValue = std::string;
using PropertyMap = std::vector<std::pair<PropertyKey, PropertyValue>>;

class MDTGNode {
public:
    MDTGNode(std::string id = {}) noexcept;
    ~MDTGNode();
    MDTGNode(const MDTGNode&) = delete;
    MDTGNode& operator=(const MDTGNode&) = delete;

    MDTGNode* addChild(std::unique_ptr<MDTGNode> child) noexcept;

    Matrix4 localTransform;
    Matrix4 globalTransform;
    PropertyMap localProps;
    PropertyMap resolvedProps;
    std::vector<double> vertices;

    const std::string& id() const noexcept { return id_; }
    MDTGNode* parent() const noexcept { return parent_; }

    void computeGlobalTransform() noexcept;
    void resolveProperties() noexcept;
    void evaluateNode();

    std::vector<MDTGNode*> takeChildrenSnapshot() noexcept;

private:
    std::string id_;
    MDTGNode* parent_;
    std::vector<std::unique_ptr<MDTGNode>> children_;
    mutable std::mutex childrenMutex_;
};
