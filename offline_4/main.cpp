#include "Decesiontree.h"
#include <bits/stdc++.h>
using namespace std;


int main(int argc, char *argv[]) {

    int maxDepth = 0;
    string criterion ;

    if(argc > 2){
        criterion = string(argv[1]);
        maxDepth = stoi(argv[2]);
    }else{
        cout << "Usage: " << argv[0] << " <criterion> <maxDepth>" << endl;
        return 1;
    }
    const int num_iter = 1;
   
    Dataset dataset = loadData("Datasets/adult.data");

  
    vector<int> attributes;
    for (int i = 0; i < dataset.attributeNames.size(); ++i) {
        attributes.push_back(i);
    }

    double avg_train_accuracy = 0;
    double avg_test_accuracy = 0;
    double avg_tree_depth = 0;
    double avg_num_nodes = 0;


    for(int i = 0; i< num_iter; i++){
        auto [trainSet, testSet] = trainTestSplit(dataset, 0.8);
        node *root = buildtree(trainSet.instances, trainSet, attributes, criterion, 0, maxDepth);
        double testAccuracy = accuracy(root, testSet.instances);
        double trainAccuracy = accuracy(root, trainSet.instances);
        int treeDepth = calculateTreeDepth(root);
        int numNodes = countNodes(root);
        cout << "Iteration " << i + 1 << ": Train Accuracy = " << trainAccuracy * 100 
             << "%, Test Accuracy = " << testAccuracy * 100 << "%" << endl;
        avg_train_accuracy += trainAccuracy;
        avg_test_accuracy += testAccuracy;
        avg_tree_depth += treeDepth;
        avg_num_nodes += numNodes;
        delete root;
    }
    avg_train_accuracy /= num_iter;
    avg_test_accuracy /= num_iter;
    avg_tree_depth /= num_iter;
    avg_num_nodes /= num_iter;
    cout << "Average Tree Depth: " << avg_tree_depth << endl;
    cout << "Average Number of Nodes: " << (int)avg_num_nodes << endl;
    cout << "Average Train Accuracy: " << avg_train_accuracy * 100 << "%" << endl;
    cout << "Average Test Accuracy: " << avg_test_accuracy * 100 << "%"<<endl;
    
    return 0;
}