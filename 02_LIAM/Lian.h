#include <vector>
#include <map>
#include "Node.h"
#include "Grid.h"

class Lian{
  const Grid& grid;
  int delta = 20;
  double alpha = 15;

  std::vector<Node*> getNeighbors(Node* currentNode);
  double calculateHeuristic(const Node& a, const Node& b); // Расчет эвристики разобраться
  std::vector<Node*> reconstructPath(Node* endNode); //Восстановления всего пути
public:
  Lian(const Grid& grid, int delta, double alphaDegrees);
   // Основной метод поиска пути
  std::vector<Node*> findPath(const Node& start, const Node& end);
};
