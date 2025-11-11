#include "../include/Dijkstra.h"
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <limits>

Dijkstra::Dijkstra(const Graph<Edge>& agraph) : graph(agraph) {}

Way Dijkstra::shortestWay(Node* startNode, Node* endNode) {
    std::map<Node*, double> distances;
    std::map<Node*, Node*> predecessors;
    std::priority_queue<std::pair<double, Node*>, std::vector<std::pair<double, Node*>>, std::greater<std::pair<double, Node*>>> pq;

    for (const auto& pair : graph.getGraph()) {
        distances[pair.first] = std::numeric_limits<double>::infinity();
        predecessors[pair.first] = nullptr;
    }

    distances[startNode] = 0;
    pq.push({0, startNode});

    while (!pq.empty()) {
        Node* u = pq.top().second;
        pq.pop();

        if (u == endNode) {
            break; 
        }

        if (distances[u] == std::numeric_limits<double>::infinity()) {
            continue;
        }

        for (const auto& edge : graph.getNeighbors(u)) {
            Node* v = edge.getEnd();
            double weight = edge.getWeight();
            if (distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
                predecessors[v] = u;
                pq.push({distances[v], v});
            }
        }
    }

    Way path;
    if (distances[endNode] != std::numeric_limits<double>::infinity()) {
        path.length = distances[endNode];
        Node* current = endNode;
        while (current != nullptr) {
            path.nodes.push_back(current);
            current = predecessors[current];
        }
        std::reverse(path.nodes.begin(), path.nodes.end());
    }

    return path;
}
