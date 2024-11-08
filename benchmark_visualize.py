import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Dataset as a multiline string
data = """
Function,Threads,Best Rate MB/s,Avg time,Min time,Max time
Copy,1,1647.3,0.098357,0.097128,0.102731
Scale,1,1687.8,0.096480,0.094799,0.099545
Add,1,2535.2,0.097152,0.094669,0.103561
Triad,1,2394.2,0.104897,0.100244,0.120685
Copy,2,3250.5,0.051475,0.049224,0.057530
Scale,2,3303.1,0.053233,0.048440,0.069565
Add,2,4951.1,0.056219,0.048474,0.073414
Triad,2,4662.8,0.055070,0.051471,0.070365
Copy,4,4866.6,0.041561,0.032877,0.058097
Scale,4,4466.4,0.044753,0.035823,0.060565
Add,4,5964.3,0.048232,0.040239,0.073115
Triad,4,5455.0,0.052225,0.043996,0.076386
Copy,8,4774.6,0.038737,0.033511,0.047776
Scale,8,4404.8,0.042031,0.036324,0.051085
Add,8,5845.8,0.045681,0.041055,0.053703
Triad,8,5387.3,0.049466,0.044549,0.060380
Copy,16,4136.2,0.048627,0.038683,0.058947
Scale,16,3992.2,0.051395,0.040078,0.070753
Add,16,5615.0,0.056593,0.042743,0.073637
Triad,16,5286.7,0.061384,0.045397,0.084140
"""

# Load the dataset into a DataFrame
from io import StringIO
df = pd.read_csv(StringIO(data))

# Set the plot style
sns.set(style="whitegrid")

# Create a figure with subplots
fig, axes = plt.subplots(2, 2, figsize=(18, 14))

# Define the metrics to plot
metrics = ['Best Rate MB/s', 'Avg time', 'Min time', 'Max time']
titles = ['Best Rate MB/s', 'Average Time', 'Minimum Time', 'Maximum Time']

# Extract handles and labels for shared legend
handles, labels = None, None

# Plot each metric in a separate subplot
for i, ax in enumerate(axes.flat):
    barplot = sns.barplot(data=df, x='Threads', y=metrics[i], hue='Function', ax=ax)
    # ax.set_title(titles[i], fontsize=12)  # Title font size
    ax.set_xlabel('Number of Threads', fontsize=12)  # X-axis label font size
    ax.set_ylabel(titles[i], fontsize=12)  # Y-axis label font size
    ax.tick_params(axis='x', rotation=45, labelsize=10)  # Rotate x-axis labels
    ax.tick_params(axis='y', labelsize=10)  # Reduce y-axis tick label size
    if i == 0:  # Get handles and labels only once
        handles, labels = ax.get_legend_handles_labels()
    ax.get_legend().remove()  # Remove individual legends

# Add a single legend to the entire figure
fig.legend(handles, labels, loc='upper center', fontsize=12, title='Function', title_fontsize=14, ncol=4)

# Adjust layout and add spacing between subplots
plt.subplots_adjust(wspace=0.3, hspace=0.4, top=0.85)

# Show the plots
plt.show()
