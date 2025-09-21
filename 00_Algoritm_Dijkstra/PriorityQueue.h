#ifndef PRIORITY_QUEUE
#define PRIORITY_QUEUE

#include <vector>
#include "Node.h"

struct MarkedNode
{
  Node* node;
  int mark;
  Node* prev;
  MarkedNode(Node* node, int mark, Node* prev);
};

class PriorityQueue
{
public:
  void push(Node* node, int mark, Node* prev);
  MarkedNode pop();
  bool empty() const;
private:
  std::vector<MarkedNode> nodes;
};
#endif
