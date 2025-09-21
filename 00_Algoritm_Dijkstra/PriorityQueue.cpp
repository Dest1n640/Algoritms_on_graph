#include "Node.h"
#include <vector>
#include "Node.h"
#include "PriorityQueue.h"


MarkedNode::MarkedNode(Node* anode = 0, int amark = 0, Node* aprev = 0) : node(anode), mark(amark), prev(aprev){}

bool PriorityQueue::empty() const{
  return nodes.empty();
}

MarkedNode PriorityQueue::pop(){
  MarkedNode best_node = nodes.back();
  nodes.pop_back();
  return best_node;
}

void PriorityQueue::push(Node* node, int mark, Node* prev){
  MarkedNode new_node(node, mark, prev);
  std::vector<MarkedNode>::iterator it = nodes.begin();
  while(it != nodes.end() && new_node.mark < it -> mark){
    it++;
  };
  if(it == nodes.end())
    nodes.push_back(new_node);
  else nodes.insert(it, new_node);
}
