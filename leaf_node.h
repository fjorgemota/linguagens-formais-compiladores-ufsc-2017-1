#include "node.h"

class LeafNode : public Node {
  public:
    using Node::Node;

    list<NodeAction> ascend();
    list<NodeAction> descend();
    NodeType getType();
};
