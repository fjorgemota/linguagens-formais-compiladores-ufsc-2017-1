#include "node.h"

class PlusNode : Node {
  public:
      list<NodeAction> ascend();
      list<NodeAction> descend();
      NodeType getType();
};
