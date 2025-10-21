#include "Edge.h"
#include <ostream>

Edge::Edge(Node* b, Node* e, int w) : begin(b), end(e), weight(w) {}

AntEdge::AntEdge(Node* b, Node* e, int w, double p)
    : Edge(b, e, w), pheromone(p) {}

Node* Edge::getBegin() const{return begin;}

Node* Edge::getEnd() const { return end; }

int Edge::getWeight() const { return weight; }

double AntEdge::getPheromone() const { return pheromone; }

void AntEdge::setPheromone(double p){
  pheromone = p;
} 

std::ostream& operator<<(std::ostream& os, const Edge& edge) {
  os << "-> " << edge.getEnd()->getName() << " (Weight: " << edge.getWeight() << ")";
  return os;
}

std::ostream& operator<<(std::ostream& os, const AntEdge& edge) {
    os << "-> " << edge.getEnd()->getName() 
       << " (Weight: " << edge.getWeight() 
       << ", Pheromone: " << edge.getPheromone() << ")";
    return os;
}
