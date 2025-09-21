#include "Graph.h"
#include "Node.h"
#include "Dijkstra.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <map>


Graph makeGraph(const std::string& file_name){
  Graph graph;

  std::ifstream inputFile(file_name);
  if (!inputFile.is_open()){
      std::cout << "Error" << std::endl;
      return graph;
  }

  std::map<std::string, Node*> nodeRegistry;;
  std::string line;

  std::getline(inputFile, line);
  
  while(std::getline(inputFile, line)){
    std::istringstream iss(line);
    std::string startName, endName;
    int weight;
    
    if (iss >> startName >> endName >> weight){
      Node* startNode = nullptr;
      Node* endNode = nullptr;

      if (nodeRegistry.find(startName) == nodeRegistry.end()){
        startNode = new Node(startName);
        nodeRegistry[startName] = startNode;
        graph.addNode(startNode);
      }
      else
        startNode = nodeRegistry[startName];

      if (nodeRegistry.find(endName) == nodeRegistry.end()){
        endNode = new Node(endName);
        nodeRegistry[endName] = endNode;
        graph.addNode(endNode);
      }
      else
        endNode = nodeRegistry[endName];

      graph.addEdge(startNode, endNode, weight);
    }

  }
  inputFile.close();
  return graph;
}

int main(){
    const std::string filename = "1000.txt";
    Graph myGraph = makeGraph(filename);

    std::cout << myGraph;
    std::string startName, endName;

    std::cout << "Введите название начальной вершины: ";
    std::cin >> startName;
    std::cout << "Введите название конечной вершины:  ";
    std::cin >> endName;

    Node* startNode = nullptr;
    Node* endNode = nullptr;

    for (Graph::graph_iterator it = myGraph.begin(); it != myGraph.end(); ++it) {
        if ((*it)->getName() == startName) {
            startNode = *it;
        }
        if ((*it)->getName() == endName) {
            endNode = *it;
        }
    }

    // --- ШАГ 5: Проверка и запуск алгоритма ---
    if (startNode == nullptr || endNode == nullptr) {
        std::cerr << "Ошибка: Один или оба узла ('" << startName << "', '" << endName << "') не найдены в графе." << std::endl;
        return 1; // Завершаем программу с кодом ошибки
    }

    // Создаем объект Дейкстры, передавая ему нашу "карту" (граф)
    Dijkstra dijkstra_solver(myGraph);
    
    std::cout << "\nЗапускаем поиск кратчайшего пути...\n";
    // Запускаем поиск и получаем результат
    Way path = dijkstra_solver.shortestWay(startNode, endNode);

    // --- ШАГ 6: Вывод результата ---
    std::cout << "\n--- Результаты поиска ---\n";
    if (path.length < 0) {
        std::cout << "Путь из '" << startName << "' в '" << endName << "' не найден.\n";
    } else {
        std::cout << "Кратчайший путь найден!\n";
        std::cout << "Общий вес (длина) пути: " << path.length << "\n";
        std::cout << "Путь: ";
        for (size_t i = 0; i < path.nodes.size(); ++i) {
            std::cout << path.nodes[i]->getName();
            if (i < path.nodes.size() - 1) {
                std::cout << " -> ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "---------------------------\n";

    return 0;
}

