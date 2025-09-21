#include <set>
#include <iostream>
#include <ostream>
#include "Graph.h"

void Graph::addNode(Node* node){
  nodes.insert(node);
}
void Graph::removeNode(Node* node){
  nodes.erase(node);
  for(graph_iterator it = nodes.begin(); it != nodes.end(); it++)
    (*it) -> removeNeighbours(node);
}

bool Graph::is_exist(Node* begin, Node* end){
  if (nodes.find(begin) == nodes.end() || nodes.find(end) == nodes.end())
    return true;  
  return false;
}

void Graph::addEdge(Node* begin, Node* end, int weight){
  if (is_exist(begin, end))
    return;
  begin ->addNeighbours(end, weight);
  end -> addNeighbours(begin, weight);
}

void Graph::removeEdge(Node* begin, Node* end){
  if (is_exist(begin, end))
    return
  begin -> removeNeighbours(end);
  end -> removeNeighbours(begin);
}


Graph::graph_iterator Graph::begin() const{
  return nodes.begin();
}

Graph::graph_iterator Graph::end() const{
  return nodes.end();
}


std::ostream& operator<<(std::ostream& os, const Graph& graph) {
    os << "--- Содержимое графа (Список смежности) ---\n";

    if (graph.begin() == graph.end()) {
        os << "Граф пуст.\n";
    }
    
    else {
        for (Graph::graph_iterator it = graph.begin(); it != graph.end(); ++it) {
            Node* currentNode = *it;
            os << "Узел [" << currentNode->getName() << "] соединен с:\n";

            // Проверяем, есть ли у узла соседи
            if (currentNode->nb_begin() == currentNode->nb_end()) {
                os << "  (Нет соседей)\n";
            }
            
            else {
                // Проходим по всем соседям текущего узла
                for (Node::node_iterator n_it = currentNode->nb_begin(); n_it != currentNode->nb_end(); ++n_it) {
                    Node* neighbor = n_it->first;
                    int weight = n_it->second;
                    os << "  -> " << neighbor->getName() << " (вес: " << weight << ")\n";
                }
            }
        }
    }
    return os;
}
