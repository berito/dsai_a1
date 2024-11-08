import pandas as pd
import matplotlib.pyplot as plt

# Load the data
data = pd.read_csv("../csv/q1_b_results.csv")

# Plot slow growth efficiency
plt.plot(data["Problem Size"], data["Efficiency (slow growth)"], label="Slow Growth", marker="o")

# Plot fast growth efficiency
plt.plot(data["Problem Size"], data["Efficiency (fast growth)"], label="Fast Growth", marker="o")

# Add labels and legend
plt.xlabel("Problem Size")
plt.ylabel("Parallel Efficiency")
plt.title("Parallel Efficiency vs Problem Size")
plt.legend()
plt.grid(True)

# Show the plot
plt.show()
