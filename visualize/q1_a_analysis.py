import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Read the CSV file
data = pd.read_csv("../csv/q1_a_results.csv")

# Plot Speedup vs. p for different values of n
plt.figure(figsize=(10, 6))
for n in data['n'].unique():
    subset = data[data['n'] == n]
    plt.plot(subset['p'], subset['Speedup'], marker='o', label=f'n = {n}')

plt.xscale('log', base=2)  # Logarithmic scale for p
plt.xlabel('Number of processors (p)')
plt.ylabel('Speedup')
plt.title('Speedup vs. Number of Processors')
plt.legend()
plt.grid(True, which="both", linestyle='--', linewidth=0.5)
plt.show()

# Plot Efficiency vs. p for different values of n
plt.figure(figsize=(10, 6))
for n in data['n'].unique():
    subset = data[data['n'] == n]
    plt.plot(subset['p'], subset['Efficiency'], marker='o', label=f'n = {n}')

plt.xscale('log', base=2)  # Logarithmic scale for p
plt.xlabel('Number of processors (p)')
plt.ylabel('Efficiency')
plt.title('Efficiency vs. Number of Processors')
plt.legend()
plt.grid(True, which="both", linestyle='--', linewidth=0.5)
plt.show()
