import pandas as pd
import matplotlib.pyplot as plt


csv_file = "2105085_1_54.csv"
df = pd.read_csv(csv_file)


graphs = df['Problem Name'].tolist()
x = range(1, len(graphs) + 1)


randomized = df['Simple Randomized or Randomized-1'].tolist()
greedy = df['Simple Greedy or Greedy-1'].tolist()
semi_greedy = [float(val.split(' (alpha=')[0]) for val in df['Semi-greedy or Greedy-1'].tolist()]
local = df['Simple local or local-1'].tolist()
grasp = df['GRASP-1'].tolist()


plt.figure(figsize=(12, 6))
plt.plot(x, randomized, label='Randomized', marker='o')
plt.plot(x, greedy, label='Greedy', marker='s')
plt.plot(x, semi_greedy, label='Semi-Greedy', marker='^')
plt.plot(x, local, label='Local Search', marker='d')
plt.plot(x, grasp, label='GRASP', marker='x')

plt.title('Max Cut (Graph 1-54)')
plt.xlabel('Graph Index')
plt.ylabel('Cut Value')
plt.xticks(x[::5], graphs[::5], rotation=45)  # Show every 5th label for readability
plt.grid(True)
plt.legend()
plt.tight_layout()


plt.savefig('maxcut_graph_1_54.png')
plt.close()