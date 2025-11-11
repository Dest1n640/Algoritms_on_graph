#ifndef GRAPH_H
#define GRAPH_H
#include "Edge.h"
#include "Node.h"
#include <map>
#include <vector>
#include <ostream>
#include <memory> // Для использования умных указателй

template <typename EdgeType> class Graph{
  std::map<std::string, std::unique_ptr<Node>> nodes; //Нужен что бы быстро обращаться
                                                      //к объектам Node по имени
  std::map<Node*, std::vector<EdgeType>> graph; //Сам граф

public:
  Node* addNode(const std::string& name);
  template<typename... Args> //Это позволяет создавать одностороние ребра
  void addEdgeOne(const std::string& beginName, const std::string& endName, Args... args);
  template <typename...Args> //Двухстороние ребра
  void addEdge(const std::string& beginName, const std::string& endName, Args... args);
  //Args... используется что бы можно было передавать n количество параметров
  //В зависимости от объекта ребра которое выбранно

//  void removeNode(const std::string& name);

  void removeEdge(const std::string& beginName, const std::string& endName);
  const std::vector<EdgeType>& getNeighbors(Node* node) const;
  const std::map<Node*, std::vector<EdgeType>>& getGraph() const;
  std::map<Node*, std::vector<EdgeType>>& getGraphNonConst();
  Node* findNode(const std::string& name) const;
  EdgeType* findEdge(Node* start, Node* end); // Метод для поиска ребра между двумя узлами

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
void Graph<EdgeType>::addEdgeOne(const std::string& beginName, const std::string& endName, Args... args){
  Node* beginNode = addNode(beginName);
  Node* endNode = addNode(endName);

  graph[beginNode].emplace_back(beginNode, endNode, args...);
}

// template<typename EdgeType>
// void Graph<EdgeType>::removeNode(const std::string& name){
//  Node* node = findNode(name);
//  if (!node) return;
//  std::vector<Node*> neighbours = getNeighbors(node);
//  for (int i = 0; i < neighbours.size(); i++) {
//      removeEdge(node->getName(), neighbours[i]->getName());
//      removeEdge(neighbours[i]->getName(), node->getName());
// }
//  graph.erase(node);
//  nodes.erase(node);
// }

template<typename EdgeType>
void Graph<EdgeType>::removeEdge(const std::string& beginName, const std::string& endName){
  Node* startNode = findNode(beginName);
  Node* endNode = findNode(endName);

  auto& edges = graph[startNode];
  for (auto it = edges.begin(); it != edges.end(); ++it) {
      if (it->getBegin() == startNode && it->getEnd() == endNode) {
          edges.erase(it);
          break;
      }
  }
}

template <typename EdgeType>
template <typename... Args>
void Graph<EdgeType>::addEdge(const std::string& beginName, const std::string& endName, Args... args) {
    Node* beginNode = addNode(beginName);
    Node* endNode = addNode(endName);

    // Создаем ребра, передавая все дополнительные аргументы
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
EdgeType* Graph<EdgeType>::findEdge(Node* start, Node* end) {
    if (graph.count(start)) {
        for (auto& edge : graph.at(start)) {
            if (edge.getEnd() == end) {
                return &edge;
            }
        }
    }
    return nullptr;
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

template<typename EdgeType>
Node* Graph<EdgeType>::findNode(const std::string& name) const {
    auto it = nodes.find(name);
    if (it != nodes.end()) {
        return it->second.get();
    }
    return nullptr;
}




#endif //GRAPH_H
