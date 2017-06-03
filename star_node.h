#include "node.h"

class StarNode : public Node {
  public:
    using Node::Node;

    list<NodeAction> ascend();
    list<NodeAction> descend();
    NodeType getType();
};
