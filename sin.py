import numpy as np
import matplotlib.pyplot as plt

# Параметры
mean_value = 3600
amplitude = 2880
num_samples = 51  # количество точек на графике

# Генерация значений от 0 до 2pi
x = np.linspace(0, 2 * np.pi, num_samples)

# Создание синусоид
sin_1 = mean_value + amplitude * np.sin(x)
sin_2 = mean_value + amplitude * np.sin(x + np.deg2rad(120))  # с начальной фазой +120 градусов
sin_3 = mean_value + amplitude * np.sin(x - np.deg2rad(120))  # с начальной фазой -120 градусов


print(np.array2string(np.round(sin_1).astype(int), separator=", "))
print(np.array2string(np.round(sin_2).astype(int), separator=", "))
print(np.array2string(np.round(sin_3).astype(int), separator=", "))

# Подготовка для графика
plt.figure(figsize=(10, 9))
plt.plot(x, sin_1, label='Sinus 1 (0° phase)')
plt.plot(x, sin_2, label='Sinus 2 (+120° phase)')
plt.plot(x, sin_3, label='Sinus 3 (-120° phase)')

# Форматирование графика
plt.title('Sine Waves')
plt.xlabel('x (radians)')
plt.ylabel('Amplitude')
plt.axhline(mean_value, color='gray', linestyle='--', label='Mean Value (200)')
plt.xticks(np.arange(0, 2 * np.pi + np.pi / 2, np.pi / 2),
           ['0', 'π/2', 'π', '3π/2', '2π'])
plt.legend()
plt.grid(True)
plt.ylim(0, mean_value + amplitude + 100)  # Ося по Y от 0 до max значения
# plt.show()

print(np.round(3.5).astype(int))