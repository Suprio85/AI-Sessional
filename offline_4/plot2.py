import pandas as pd
import matplotlib.pyplot as plt


plt.style.use('seaborn-v0_8')


data = pd.read_csv('csv/prun_vs_no_prun.csv')


criteria = data['Criterion'].unique()
metrics = ['Avg_Test_Accuracy', 'Avg_Train_Accuracy', 'Avg_Tree_Depth', 'Avg_Num_Nodes']
max_depths = [0, 5]  


fig, axes = plt.subplots(2, 2, figsize=(12, 10), dpi=100)
axes = axes.flatten()


colors = ['#FF9999', '#66B2FF']

for idx, metric in enumerate(metrics):
    ax = axes[idx]
    x = range(len(criteria))
    width = 0.35

    for i, depth in enumerate(max_depths):
        values = data[data['MaxDepth'] == depth][metric]
        ax.bar([p + width * i for p in x], values, width, label=f'MaxDepth={depth}', color=colors[i])

    ax.set_title(f'{metric.replace("_", " ").title()}', fontsize=12)
    ax.set_xticks([p + width / 2 for p in x])
    ax.set_xticklabels(criteria)
    ax.set_xlabel('Criterion', fontsize=10)
    ax.set_ylabel(metric.replace("_", " ").title(), fontsize=10)
    ax.legend()
    ax.grid(True, linestyle='--', alpha=0.7)

    
    if metric in ['Avg_Test_Accuracy', 'Avg_Train_Accuracy']:
        ax.set_ylim(0.7, 1.0)
    elif metric == 'Avg_Tree_Depth':
        ax.set_ylim(0, 600)
    else:  # Avg_Num_Nodes
        ax.set_yscale('log')
        ax.set_ylim(10, 15000)


plt.tight_layout()


plt.savefig('plots/prun_vs_no_prun_visualization.png', format='png', dpi=300)
plt.close()
