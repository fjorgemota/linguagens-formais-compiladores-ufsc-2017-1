#include "node.h"

class QuestionMarkNode : public Node {
  public:
    using Node::Node;

    list<NodeAction> ascend();
    list<NodeAction> descend();
    NodeType getType();
};
