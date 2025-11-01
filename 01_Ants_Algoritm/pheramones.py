import matplotlib.pyplot as plt

def plot_pheromone_history(filename="output.txt"):
    """
    Считывает данные из файла и строит график зависимости 
    общего количества феромона от итерации.
    """
    iterations = []
    pheromones = []

    try:
        with open(filename, 'r') as f:
            for line in f:
                # Пропускаем строки, которые являются комментариями или пустые
                if line.strip().startswith('#') or not line.strip():
                    continue
                
                parts = line.strip().split()
                if len(parts) >= 2:
                    try:
                        # Добавляем данные в списки
                        iterations.append(int(parts[0]))
                        pheromones.append(float(parts[1]))
                    except ValueError:
                        # Пропускаем строки с некорректным форматом данных
                        print(f"Предупреждение: не удалось обработать строку '{line.strip()}'")
                        continue

        if not iterations:
            print(f"Файл '{filename}' пуст или не содержит данных для построения графика.")
            return

        # Создание графика
        plt.figure(figsize=(10, 6))
        plt.plot(iterations, pheromones, marker='o', linestyle='-', markersize=4)
        
        # Настройка графика
        plt.title('Динамика общего количества феромона')
        plt.xlabel('Итерация')
        plt.ylabel('Общее количество феромона')
        plt.grid(True)
        plt.tight_layout()
        
        # Показать график
        plt.show()

    except FileNotFoundError:
        print(f"Ошибка: Файл '{filename}' не найден.")
    except Exception as e:
        print(f"Произошла ошибка при обработке файла или построении графика: {e}")

if __name__ == "__main__":
    plot_pheromone_history()
