#ifndef GRAPH_H
#define GRAPH_H
#include "Edge.h"
#include "Node.h"
#include <map>
#include <vector>
#include <ostream>
#include <memory> // Для использования умных указателй
// Использованны, чтобы избежать утечек памяти и упростить управление временем жизни объектов.

template <typename EdgeType> class Graph{
  std::map<std::string, std::unique_ptr<Node>> nodes; //Нужен что бы быстро обращаться
                                                      //к объектам Node по имени
  std::map<Node*, std::vector<EdgeType>> graph;
public:
  Node* addNode(const std::string& name);
  template<typename... Args>
  void addEdge(const std::string& beginName, const std::string& endName, Args... args);
  const std::vector<EdgeType>& getNeighbors(Node* node) const;
  const std::map<Node*, std::vector<EdgeType>>& getGraph() const;
  std::map<Node*, std::vector<EdgeType>>& getGraphNonConst();

};
template<typename EdgeType>
Node* Graph<EdgeType>::addNode(const std::string& name){
  auto [it, inserted] = nodes.emplace(name, std::make_unique<Node>(name));
  if (inserted) {
    graph[it->second.get()] = std::vector<EdgeType>();
  }
  return it->second.get();
}

template <typename EdgeType>
template <typename... Args>
void Graph<EdgeType>::addEdge(const std::string& beginName, const std::string& endName, Args... args) {
    Node* beginNode = addNode(beginName);
    Node* endNode = addNode(endName);

    // Создаем ребра, передавая все дополнительные аргументы в конструктор EdgeType
    graph[beginNode].emplace_back(beginNode, endNode, args...);
    graph[endNode].emplace_back(endNode, beginNode, args...);
}

template<typename EdgeType>
const std::vector<EdgeType>& Graph<EdgeType>::getNeighbors(Node* node) const{
  auto it = graph.find(node);
  if (it != graph.end()) {
    return it->second;
  }
  static const std::vector<EdgeType> empty;
  return empty;
}
template<typename EdgeType>
const std::map<Node*, std::vector<EdgeType>>& Graph<EdgeType>::getGraph() const{
  return graph;
}
template<typename EdgeType>
std::map<Node*, std::vector<EdgeType>>& Graph<EdgeType>::getGraphNonConst(){
  return graph;
}


template<typename EdgeType>
std::ostream& operator<<(std::ostream& os, const Graph<EdgeType>& g) {
    for (const auto& pair : g.getGraph()) {
        Node* node = pair.first;
        const auto& neighbors = pair.second;
        os << "Node " << node->getName() << ":\n";
        if (neighbors.empty()) {
            os << "  (No neighbors)\n";
        } else {
            for (const auto& edge : neighbors) {
                os << "  " << edge << "\n";
            }
        }
    }
    return os;
}





#endif //GRAPH_H
