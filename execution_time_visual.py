import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Step 1: Read the data
df = pd.read_csv('execution_times.csv')

# Step 2: Set up the plot
plt.figure(figsize=(12, 8))

# Step 3: Prepare the bar positions
threads = df['Threads'].unique()
versions = df['Version'].unique()
bar_width = 0.25  # Width of each bar
positions = np.arange(len(threads))  # Base positions for each thread count

# Step 4: Plot each version's bars
for i, version in enumerate(versions):
    # Filter data for each version
    subset = df[df['Version'] == version]
    
    # Adjust positions for each version's bars to group them together
    plt.bar(positions + i * bar_width, subset['ExecutionTime'], width=bar_width, label=version)

# Step 5: Customize the plot
plt.title('Execution Time vs Number of Threads by Version', fontsize=16)
plt.xlabel('Number of Threads', fontsize=14)
plt.ylabel('Execution Time (seconds)', fontsize=14)

# Set x-ticks to be in the center of the grouped bars
plt.xticks(positions + bar_width, threads, fontsize=12)
plt.legend(title='Version')

# Show the plot
plt.show()
