#include "node.h"

class LambdaNode : Node {
  public:
      list<NodeAction> ascend();
      list<NodeAction> descend();
      NodeType getType();
};
