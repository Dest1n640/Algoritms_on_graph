#include <vector>
#include <map>
#include <algorithm>
#include "Node.h"
#include "Dijkstra.h"
#include "PriorityQueue.h"
#include "Graph.h"

Dijkstra::Dijkstra(const Graph&  agraph) : graph(agraph){}

Way Dijkstra::unroll(const std::map<Node*, MarkedNode>& visited, Node* begin, Node* curr) {
    Way way;

    // 2. Используем .at() для const-безопасного ЧТЕНИЯ
    way.length = visited.at(curr).mark;

    while (curr != nullptr) { // Проверяем на nullptr, а не на begin
        way.nodes.push_back(curr);
        // Снова используем .at()
        curr = visited.at(curr).prev;
    }

    // 3. Разворачиваем путь
    std::reverse(way.nodes.begin(), way.nodes.end());

    return way;
}

Way Dijkstra::shortestWay(Node* begin, Node* end){
  PriorityQueue nodes;
  nodes.push(begin, 0, 0);
  std::map<Node*, MarkedNode> visited;

  while(!nodes.empty()){
    MarkedNode next = nodes.pop();
    visited.insert({next.node, next});

    if(end==next.node)
      return unroll(visited, begin, end); //Раскрутка

    for (Node::node_iterator it = next.node->nb_begin(); it != next.node->nb_end(); ++it) { 
      Node* neighbourd_node = it ->first;
      int edge_weight = it -> second;

      int new_total_weight = next.mark + edge_weight;

      if(visited.find(neighbourd_node) == visited.end())
        nodes.push(neighbourd_node, new_total_weight, next.node);
    }
  }
  return Way();
}
