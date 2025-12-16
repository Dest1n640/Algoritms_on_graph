import matplotlib.pyplot as plt

iteration = []
pher_per_path = []
best_path_pheromone = []
best_path_length = []
current_path_length = []

with open('../data/output.txt', 'r') as f:
    for line in f:
        parts = line.strip().split()
        if len(parts) >= 2 and parts[0].replace('.', '', 1).isdigit():
            iteration.append(float(parts[0]))
            pher_per_path.append(float(parts[1]))
            if len(parts) >= 3:
                best_path_pheromone.append(float(parts[2]))
            if len(parts) >= 4:
                best_path_length.append(float(parts[3]))
            if len(parts) >= 5:
                current_path_length.append(float(parts[4]))

# Создаем два графика
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8))

# График феромонов - два графика на одном
ax1.plot(iteration, pher_per_path, 'b-', label='Total Pheromone')
if best_path_pheromone:
    ax1.plot(iteration[:len(best_path_pheromone)], best_path_pheromone, 'orange', linewidth=2, label='Best Path Pheromone')
ax1.set_xlabel('Iteration')
ax1.set_ylabel('Total Pheromone')
ax1.set_title('Pheromone Evolution')
ax1.grid(True)
ax1.legend()

# График длины пути - два графика на одном
if best_path_length:
    ax2.plot(iteration[:len(best_path_length)], best_path_length, 'g-', linewidth=2, label='Best Path Length (Global)')
if current_path_length:
    ax2.plot(iteration[:len(current_path_length)], current_path_length, 'r-', alpha=0.7, label='Current Path Length (Iteration)')
ax2.set_xlabel('Iteration')
ax2.set_ylabel('Path Length')
ax2.set_title('Path Length Evolution')
ax2.grid(True)
ax2.legend()

plt.tight_layout()
plt.show()

