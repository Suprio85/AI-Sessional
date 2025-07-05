import pandas as pd
import matplotlib.pyplot as plt
import os


plt.style.use('seaborn-v0_8')


ig_data = pd.read_csv('ig_iris.csv')
igr_data = pd.read_csv('igr_iris.csv')
nwig_data = pd.read_csv('nwig_iris.csv')

# ig_data = pd.read_csv('IG.csv')
# igr_data = pd.read_csv('IGR.csv')
# nwig_data = pd.read_csv('NWIG.csv')

ig_data = ig_data[ig_data['MaxDepth'].isin(range(1, 16))]
igr_data = igr_data[igr_data['MaxDepth'].isin(range(1, 16))]
nwig_data = nwig_data[nwig_data['MaxDepth'].isin(range(1, 16))]


fig, (ax1, ax2, ax3) = plt.subplots(1, 3, figsize=(18, 6), dpi=100)


ax1.plot(ig_data['MaxDepth'], ig_data['Avg_Test_Accuracy'], marker='o', label='IG', color='blue')
ax1.plot(igr_data['MaxDepth'], igr_data['Avg_Test_Accuracy'], marker='s', label='IGR', color='green')
ax1.plot(nwig_data['MaxDepth'], nwig_data['Avg_Test_Accuracy'], marker='^', label='NWIG', color='red')

ax1.set_title('Average Test Accuracy vs Max Tree Depth', fontsize=14, pad=10)
ax1.set_xlabel('Max Tree Depth', fontsize=12)
ax1.set_ylabel('Average Test Accuracy', fontsize=12)
ax1.set_xticks(range(1, 16, 2))
ax1.grid(True, linestyle='--', alpha=0.7)
ax1.legend()
ax1.set_ylim(0.7, 0.9)

ax2.plot(ig_data['MaxDepth'], ig_data['Avg_Train_Accuracy'], marker='o', label='IG', color='blue')
ax2.plot(igr_data['MaxDepth'], igr_data['Avg_Train_Accuracy'], marker='s', label='IGR', color='green')
ax2.plot(nwig_data['MaxDepth'], nwig_data['Avg_Train_Accuracy'], marker='^', label='NWIG', color='red')

ax2.set_title('Average Train Accuracy vs Max Tree Depth', fontsize=14, pad=10)
ax2.set_xlabel('Max Tree Depth', fontsize=12)
ax2.set_ylabel('Average Train Accuracy', fontsize=12)
ax2.set_xticks(range(1, 16, 2))
ax2.grid(True, linestyle='--', alpha=0.7)
ax2.legend()
ax2.set_ylim(0.7, 1.0)


ax3.plot(ig_data['MaxDepth'], ig_data['Avg_Num_Nodes'], marker='o', label='IG', color='blue')
ax3.plot(igr_data['MaxDepth'], igr_data['Avg_Num_Nodes'], marker='s', label='IGR', color='green')
ax3.plot(nwig_data['MaxDepth'], nwig_data['Avg_Num_Nodes'], marker='^', label='NWIG', color='red')

ax3.set_title('Average Number of Nodes vs Max Tree Depth', fontsize=14, pad=10)
ax3.set_xlabel('Max Tree Depth', fontsize=12)
ax3.set_ylabel('Average Number of Nodes', fontsize=12)
ax3.set_xticks(range(1, 16, 2))
ax3.grid(True, linestyle='--', alpha=0.7)
ax3.legend()
ax3.set_yscale('log')  


plt.tight_layout()

plt.savefig('iris_decision_tree_analysis.png', dpi=300, bbox_inches='tight')
plt.show()