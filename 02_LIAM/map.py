import cv2
import numpy as np

def process_image(file_name):
    """
    Загружает изображение, находит на нем желтую (старт) и красную (финиш) точки,
    и преобразует его в бинарную сетку для поиска пути.

    Returns:
        tuple: (grid, start_coords, end_coords)
               - grid: бинарная сетка (1 - препятствие, 0 - свободно)
               - start_coords: кортеж (x, y) желтой точки
               - end_coords: кортеж (x, y) красной точки
    """
    image = cv2.imread(file_name)
    if image is None:
        raise FileNotFoundError(f"Не удалось загрузить изображение: {file_name}")

    hsv_image = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
    
    # --- Поиск желтой точки (старт) ---
    lower_yellow = np.array([100, 100, 100])
    upper_yellow = np.array([130, 255, 255])
    yellow_mask = cv2.inRange(hsv_image, lower_yellow, upper_yellow)
    yellow_contours, _ = cv2.findContours(yellow_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    
    start_coords = None
    if yellow_contours:
        c = max(yellow_contours, key=cv2.contourArea)
        M = cv2.moments(c)
        if M["m00"] != 0:
            cX = int(M["m10"] / M["m00"])
            cY = int(M["m01"] / M["m00"])
            start_coords = (cX, cY)

    # --- Поиск красной точки (финиш) ---
    lower_red1 = np.array([0, 120, 70])
    upper_red1 = np.array([10, 255, 255])
    lower_red2 = np.array([170, 120, 70])
    upper_red2 = np.array([180, 255, 255])
    red_mask = cv2.inRange(hsv_image, lower_red1, upper_red1) + cv2.inRange(hsv_image, lower_red2, upper_red2)
    red_contours, _ = cv2.findContours(red_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    end_coords = None
    if red_contours:
        c = max(red_contours, key=cv2.contourArea)
        M = cv2.moments(c)
        if M["m00"] != 0:
            cX = int(M["m10"] / M["m00"])
            cY = int(M["m01"] / M["m00"])
            end_coords = (cX, cY)
            
    # --- Создание бинарной сетки ---
    # "Стираем" цветные точки с копии изображения, чтобы они не стали препятствиями
    temp_image = image.copy()
    if start_coords:
        cv2.circle(temp_image, start_coords, 15, (255, 255, 255), -1)
    if end_coords:
        cv2.circle(temp_image, end_coords, 15, (255, 255, 255), -1)

    gray_image = cv2.cvtColor(temp_image, cv2.COLOR_BGR2GRAY)
    # Препятствия - черные, путь - белый. THRESH_BINARY_INV инвертирует это.
    # Порог 200 хорошо отделяет черные здания от светло-серого фона карты.
    _, binary_image = cv2.threshold(gray_image, 200, 255, cv2.THRESH_BINARY_INV)
    
    # Преобразуем в сетку: 1 - препятствие, 0 - свободно
    grid = (binary_image == 255).astype(int)

    return grid, start_coords, end_coords
