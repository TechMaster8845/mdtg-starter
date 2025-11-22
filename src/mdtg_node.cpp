#include "mdtg/mdtg_node.hpp"
#include "mdtg/thread_pool.hpp"
#include <algorithm>


MDTGNode::MDTGNode(std::string id) noexcept
: id_(std::move(id)), parent_(nullptr) {}

MDTGNode::~MDTGNode() = default;

MDTGNode* MDTGNode::addChild(std::unique_ptr<MDTGNode> child) noexcept {
    child->parent_ = this;
    std::lock_guard<std::mutex> l(childrenMutex_);
    children_.push_back(std::move(child));
    return children_.back().get();
}

std::vector<MDTGNode*> MDTGNode::takeChildrenSnapshot() noexcept {
    std::vector<MDTGNode*> out;
    std::lock_guard<std::mutex> l(childrenMutex_);
    for (auto& c : children_) out.push_back(c.get());
    return out;
}

void MDTGNode::computeGlobalTransform() noexcept {
    globalTransform = parent_ ? Matrix4::multiply(parent_->globalTransform, localTransform)
                              : localTransform;
}

void MDTGNode::resolveProperties() noexcept {
    resolvedProps = localProps;
    if (parent_) {
        for (auto& p : parent_->resolvedProps) {
            if (std::none_of(resolvedProps.begin(), resolvedProps.end(),
                [&](auto& kv){ return kv.first == p.first; }))
                resolvedProps.push_back(p);
        }
    }
}

void MDTGNode::evaluateNode() {
    double sum = 0.0;
    for (auto v : vertices) sum += v * 0.000001;
    (void)sum;
}
