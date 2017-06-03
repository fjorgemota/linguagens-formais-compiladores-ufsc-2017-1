#include "node.h"

class QuestionMarkNode : Node {
  public:
      list<NodeAction> ascend();
      list<NodeAction> descend();
      NodeType getType();
};
