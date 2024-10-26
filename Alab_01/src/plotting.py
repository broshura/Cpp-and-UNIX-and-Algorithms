import matplotlib.pyplot as plt
import csv

# Чтение данных из файла
sizes = []
lsd_fixed_times = []
merge_fixed_times = []
lsd_range_times = []
merge_range_times = []

with open('build/results.csv', 'r') as file:
    reader = csv.reader(file)
    next(reader)  # Пропустить заголовок
    for row in reader:
        size, lsd_fixed, merge_fixed, lsd_range, merge_range = map(float, row)
        sizes.append(int(size))
        lsd_fixed_times.append(lsd_fixed)
        merge_fixed_times.append(merge_fixed)
        lsd_range_times.append(lsd_range)
        merge_range_times.append(merge_range)

# Построение графиков
plt.figure(figsize=(10, 6))

plt.plot(sizes, lsd_fixed_times, label='LSD Fixed', marker='o', color = 'red')
plt.plot(sizes, merge_fixed_times, label='Merge Fixed', marker='o', color = 'blue')
plt.plot(sizes, lsd_range_times, label='LSD Range', marker='o', color = 'green')
plt.plot(sizes, merge_range_times, label='Merge Range', marker='o', color = 'orange')

plt.xlabel('Размер массива')
plt.yscale('log')
plt.xscale('log')
plt.ylabel('Время сортировки (секунды)')
plt.title('Сравнение времени сортировки алгоритмов LSD и Merge')
plt.legend()
plt.grid(True)
plt.savefig('build/sorting_comparison.png')
plt.show()