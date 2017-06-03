#include "node.h"

class StarNode : Node {
  public:
      list<NodeAction> ascend();
      list<NodeAction> descend();
      NodeType getType();
};
