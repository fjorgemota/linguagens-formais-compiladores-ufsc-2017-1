#include "node.h"

class UnionNode : public Node {
  public:
    using Node::Node;

    list<NodeAction> ascend();
    list<NodeAction> descend();
    NodeType getType();
};
