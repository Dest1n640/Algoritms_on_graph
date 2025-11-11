import cv2
import numpy as np
from map import process_image
from lian import Lian
import sys
import os

import time

def draw_path(image, path, color=(0, 0, 255), thickness=3):
    """Рисует найденный путь на изображении."""
    if not path or len(path) < 2:
        return image
    
    # Рисуем отрезки пути
    for i in range(len(path) - 1):
        pt1 = (int(path[i][0]), int(path[i][1]))
        pt2 = (int(path[i+1][0]), int(path[i+1][1]))
        cv2.line(image, pt1, pt2, color, thickness)
    
    # Отмечаем узловые точки пути
    for point in path:
        cv2.circle(image, (int(point[0]), int(point[1])), 4, (255, 0, 0), -1)

    return image

def main():
    # --- Параметры ---
    # Определяем путь относительно текущего файла
    script_dir = os.path.dirname(os.path.abspath(__file__))
    image_path = os.path.join(script_dir, '..', 'data', 'karta-01.bmp')
    delta = 5   # Радиус поиска (в пикселях)
    alpha_max = 30  # Максимальный угол отклонения (в градусах)

    # 1. Обрабатываем изображение: получаем сетку, старт и финиш
    try:
        print(f"Обработка изображения '{image_path}'...")
        grid, start_node, end_node = process_image(image_path)
    except FileNotFoundError as e:
        print(f"Ошибка: {e}", file=sys.stderr)
        return
    except Exception as e:
        print(f"Непредвиденная ошибка при обработке изображения: {e}", file=sys.stderr)
        return

    if start_node is None:
        print("Ошибка: Не удалось найти стартовую (синюю) точку на изображении.", file=sys.stderr)
        return
    if end_node is None:
        print("Ошибка: Не удалось найти конечную (красную) точку на изображении.", file=sys.stderr)
        return
        
    print(f"Стартовая точка найдена: {start_node}")
    print(f"Конечная точка найдена: {end_node}")

    # 2. Инициализируем и запускаем алгоритм
    print(f"Запуск алгоритма LIAN с delta={delta} и alpha_max={alpha_max}...")
    lian_solver = Lian(grid, delta=delta, alpha_max=alpha_max)
    
    start_time = time.time()
    path = lian_solver.find_path(start_node, end_node)
    end_time = time.time()
    
    print(f"Поиск завершен за {end_time - start_time:.4f} секунд.")

    # 3. Визуализация результата
    vis_image = cv2.imread(image_path)
    
    if path:
        print(f"Путь найден! Количество узлов: {len(path)}.")
        # Рисуем путь зеленым цветом
        vis_image = draw_path(vis_image, path, color=(0, 255, 0), thickness=2)
    else:
        print("Путь не найден.")

    # Отмечаем старт и финиш для наглядности
    cv2.circle(vis_image, start_node, 8, (255, 0, 0), -1)   # Синий старт
    cv2.circle(vis_image, end_node, 8, (0, 0, 255), -1)     # Красный финиш

    # Сохраняем и показываем результат
    result_path = 'result_path.png'
    cv2.imwrite(result_path, vis_image)
    print(f"Результат сохранен в '{result_path}'")
    
    cv2.imshow('Lian Algorithm Path', vis_image)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()
