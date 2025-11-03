import heapq
import math
import numpy as np

class Lian:
    def __init__(self, grid, delta=5, alpha_max=45):
        self.grid = grid
        self.height, self.width = grid.shape
        self.delta = delta
        self.alpha_max = math.radians(alpha_max) # Конвертируем в радианы

    def _is_obstacle(self, x, y):
        """Проверяет, является ли ячейка препятствием."""
        px, py = int(round(x)), int(round(y))
        if 0 <= px < self.width and 0 <= py < self.height:
            return self.grid[py, px] == 1
        return True

    def _get_circle_successors(self, node):
        """Генерирует точки на окружности радиуса Delta."""
        x_center, y_center = node
        successors = set()
        x = self.delta
        y = 0
        p = 1 - self.delta
        
        while x >= y:
            points = [
                (x_center + x, y_center + y), (x_center - x, y_center + y),
                (x_center + x, y_center - y), (x_center - x, y_center - y),
                (x_center + y, y_center + x), (x_center - y, y_center + x),
                (x_center + y, y_center - x), (x_center - y, y_center - x),
            ]
            for p_ in points:
                if 0 <= p_[0] < self.width and 0 <= p_[1] < self.height:
                    successors.add((int(p_[0]), int(p_[1])))
            
            y += 1
            if p <= 0:
                p = p + 2 * y + 1
            else:
                x -= 1
                p = p + 2 * y - 2 * x + 1
        return list(successors)

    def _heuristic(self, a, b):
        return math.sqrt((a[0] - b[0])**2 + (a[1] - b[1])**2)

    def _get_angle(self, p0, p1, p2):
        vec1 = (p1[0] - p0[0], p1[1] - p0[1])
        vec2 = (p2[0] - p1[0], p2[1] - p1[1])
        
        dot_product = vec1[0] * vec2[0] + vec1[1] * vec2[1]
        mag1 = math.sqrt(vec1[0]**2 + vec1[1]**2)
        mag2 = math.sqrt(vec2[0]**2 + vec2[1]**2)
        
        if mag1 == 0 or mag2 == 0: return 0
            
        cos_angle = max(-1.0, min(1.0, dot_product / (mag1 * mag2)))
        return math.acos(cos_angle)

    def _line_of_sight(self, a, b):
        x0, y0 = a
        x1, y1 = b
        dx = abs(x1 - x0)
        dy = -abs(y1 - y0)
        sx = 1 if x0 < x1 else -1
        sy = 1 if y0 < y1 else -1
        err = dx + dy

        while True:
            if self._is_obstacle(x0, y0): return False
            if x0 == x1 and y0 == y1: break
            e2 = 2 * err
            if e2 >= dy:
                err += dy
                x0 += sx
            if e2 <= dx:
                err += dx
                y0 += sy
        return True

    def _reconstruct_path(self, came_from, current):
        path = [current]
        while current in came_from:
            current = came_from[current]
            path.append(current)
        return path[::-1]

    def find_path(self, start, end):
        """Основной метод поиска пути LIAN."""
        open_set = [(self._heuristic(start, end), 0, start, None)] 
        came_from = {}
        g_score = {start: 0}
        closed_set = set()

        while open_set:
            _, _, current, parent = heapq.heappop(open_set)

            if current in closed_set:
                continue
            
            closed_set.add(current)

            if self._heuristic(current, end) <= self.delta and self._line_of_sight(current, end):
                came_from[end] = current
                return self._reconstruct_path(came_from, end)

            successors = self._get_circle_successors(current)

            for neighbor in successors:
                if neighbor in closed_set:
                    continue
                
                if self._is_obstacle(neighbor[0], neighbor[1]): continue
                if not self._line_of_sight(current, neighbor): continue
                
                if parent:
                    angle = self._get_angle(parent, current, neighbor)
                    if angle > self.alpha_max: continue
                
                path_g_score = g_score[current] + self._heuristic(current, neighbor)
                
                if path_g_score < g_score.get(neighbor, float('inf')):
                    came_from[neighbor] = current
                    g_score[neighbor] = path_g_score
                    f_score = path_g_score + self._heuristic(neighbor, end)
                    heapq.heappush(open_set, (f_score, path_g_score, neighbor, current))
        
        return None
