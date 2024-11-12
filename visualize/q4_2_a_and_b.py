import matplotlib.pyplot as plt
import pandas as pd

# Read data from CSV file
csv_file = '../csv/q4_2_a_and_b_results.csv'  # Specify the path to your CSV file
df = pd.read_csv(csv_file)

# Plot Speedup vs Number of Threads
plt.figure(figsize=(10, 5))

# Speedup plot
plt.subplot(1, 2, 1)
plt.plot(df['Threads'], df['Speedup'], marker='o', color='b', label='Speedup')
plt.title('Speedup vs Number of Threads')
plt.xlabel('Number of Threads')
plt.ylabel('Speedup')
plt.grid(True)
plt.xticks(df['Threads'])
plt.legend()

# Efficiency plot
plt.subplot(1, 2, 2)
plt.plot(df['Threads'], df['Efficiency'], marker='o', color='r', label='Efficiency')
plt.title('Efficiency vs Number of Threads')
plt.xlabel('Number of Threads')
plt.ylabel('Efficiency')
plt.grid(True)
plt.xticks(df['Threads'])
plt.legend()

# Show the plot
plt.tight_layout()
plt.show()
