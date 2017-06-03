#include "all.h"
#include "Node.h"

class StarNode : Node {
  public:
      list<NodeAction> ascend();
      list<NodeAction> descend();
};
