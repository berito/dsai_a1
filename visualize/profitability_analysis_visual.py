import pandas as pd
import matplotlib.pyplot as plt

# Step 1: Load the data from the CSV file
data = pd.read_csv('profitability_data.csv')

# Step 2: Plot Execution Time Comparison
plt.figure(figsize=(12, 6))
plt.bar(data['Threads'] - 0.15, data['T_serial'], width=0.3, label='Serial Execution Time', color='skyblue')
plt.bar(data['Threads'] + 0.15, data['T_parallel'], width=0.3, label='Parallel Execution Time', color='salmon')
plt.xlabel('Number of Threads')
plt.ylabel('Execution Time (seconds)')
plt.title('Execution Time: Serial vs Parallel for Version 3')
plt.xticks(data['Threads'])
plt.legend()

# Step 3: Plot Speedup Comparison with Profitability Line
plt.figure(figsize=(12, 6))
plt.plot(data['Threads'], data['Speedup'], marker='o', color='green', label='Speedup')
plt.axhline(y=1, color='red', linestyle='--', label='Profitability Threshold (Speedup=1)')
plt.xlabel('Number of Threads')
plt.ylabel('Speedup (T_serial / T_parallel)')
plt.title('Speedup vs Number of Threads for Version 3')
plt.xticks(data['Threads'])
plt.legend()

# Show both plots
plt.show()
