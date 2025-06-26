#include "Decesiontree.h"
#include <bits/stdc++.h>
using namespace std;


int main() {
    // Load dataset
    Dataset dataset = loadData("Datasets/adult.csv");
    dataset.printSummary();
    dataset.head(5);
    // Split dataset into training and testing sets
    auto [trainSet, testSet] = trainTestSplit(dataset, 0.8);
    cout << "Training set:" << endl;
    trainSet.printSummary();
    trainSet.head(5);
    cout << "Testing set:" << endl;
    testSet.printSummary();
    testSet.head(5);
    // Build decision tree
    vector<string> attributes = trainSet.attributeNames;
    string criterion = "IGR"; // Information Gain
    node* root = buildtree(trainSet.instances, trainSet, attributes, criterion, 0);
    cout << "Decision tree built successfully." << endl;

    // Evaluate on training set
    double trainAccuracy = accuracy(root, trainSet.instances);
    cout << "Training set accuracy: " << trainAccuracy << endl;

    // Evaluate on testing set
    double testAccuracy = accuracy(root, testSet.instances);
    cout << "Testing set accuracy: " << testAccuracy << endl;

    return 0;
}