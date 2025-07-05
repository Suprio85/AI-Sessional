#include "Decesiontree.h"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char *argv[]) {
    // if (argc != 2) {
    //     cout << "Usage: " << argv[0] << " <criterion>" << endl;
    //     return 1;
    // }
    // string criterion = argv[1];


    vector<int> maxDepths = {0,5};
    const int num_iter = 20;

    Dataset dataset = loadData("Datasets/adult.csv");
    imputeMissingValues(dataset);
    vector<int> attributes;
    for (int i = 0; i < dataset.attributeNames.size(); ++i) {
        attributes.push_back(i);
    }


    cout << "Criterion,MaxDepth,Avg_Train_Accuracy,Avg_Test_Accuracy,Avg_Tree_Depth,Avg_Num_Nodes" << endl;
for(const string &criterion : {"IG", "IGR", "NWIG"}) {
    for (int maxDepth : maxDepths) {
        double total_train_accuracy = 0;
        double total_test_accuracy = 0;
        double total_tree_depth = 0;
        double total_num_nodes = 0;


        for (int i = 0; i < num_iter; i++) {
            auto [trainSet, testSet] = trainTestSplit(dataset, 0.8);
            node *root = buildtree(trainSet.instances, trainSet, attributes, criterion, 0, maxDepth);
            double trainAccuracy = accuracy(root, trainSet.instances);
            double testAccuracy = accuracy(root, testSet.instances);
            int treeDepth = calculateTreeDepth(root);
            int numNodes = countNodes(root);
            total_train_accuracy += trainAccuracy;
            total_test_accuracy += testAccuracy;
            total_tree_depth += treeDepth;
            total_num_nodes += numNodes;
            delete root;
        }
        double avg_train_accuracy = total_train_accuracy / num_iter;
        double avg_test_accuracy = total_test_accuracy / num_iter;
        double avg_tree_depth = total_tree_depth / num_iter;
        double avg_num_nodes = total_num_nodes / num_iter;

        cout << criterion << "," << maxDepth << "," << avg_train_accuracy << ","
             << avg_test_accuracy << "," << avg_tree_depth << "," << avg_num_nodes << endl;
    }
}

    return 0;
}