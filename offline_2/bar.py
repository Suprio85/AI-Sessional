import pandas as pd
import matplotlib.pyplot as plt


csv_file = "2105085_1_54.csv"
df = pd.read_csv(csv_file)


graphs = df['Problem Name'].tolist()
x = range(len(graphs))  


randomized = df['Simple Randomized or Randomized-1'].tolist()
greedy = df['Simple Greedy or Greedy-1'].tolist()
semi_greedy = [float(val.split(' (alpha=')[0]) for val in df['Semi-greedy or Greedy-1'].tolist()]
local = df['Simple local or local-1'].tolist()
grasp = df['GRASP-1'].tolist()


bar_width = 0.15


r1 = [i - 2*bar_width for i in x]
r2 = [i - bar_width for i in x]
r3 = [i for i in x]
r4 = [i + bar_width for i in x]
r5 = [i + 2*bar_width for i in x]


plt.figure(figsize=(15, 6))
plt.bar(r1, randomized, color='blue', width=bar_width, label='Randomized')
plt.bar(r2, greedy, color='orange', width=bar_width, label='Greedy')
plt.bar(r3, semi_greedy, color='gray', width=bar_width, label='Semi-Greedy')
plt.bar(r4, grasp, color='yellow', width=bar_width, label='GRASP')
plt.bar(r5, local, color='purple', width=bar_width, label='Local Search')


plt.title('Max Cut (Graph 1-54)')
plt.xlabel('Graph Index')
plt.ylabel('Cut Value')
plt.xticks(x, graphs, rotation=90)  # Rotate labels for readability
plt.grid(True, axis='y', linestyle='--', alpha=0.7)
plt.legend()
plt.tight_layout()


plt.margins(x=0.01)

plt.savefig('maxcut_bar_graph_1_54.png', bbox_inches='tight')
plt.close()