#include<bits/stdc++.h>
#include "Dataloader.h"
using namespace std;

#define MAX_DEPTH 5


struct node{
    string attribute;
    int attributeIndex;
    string value;
    unordered_map<string, node*> children;
    double threshold;
    string classLabel;
    bool isLeaf;
    node* left = nullptr;
    node* right = nullptr;

    node() : isLeaf(false), threshold(0.0), attributeIndex(-1)  {
        left = nullptr;
        right = nullptr;
    }
    node(string classLabel, bool isLeafNode = true) : classLabel(classLabel), isLeaf(isLeafNode) {
        left = nullptr;
        right = nullptr;
    }
    ~node() {
        for (auto& child : children) {
            delete child.second;
        }
        children.clear();
        delete left;
        delete right;
    }
};

struct splitinfo {
    string attribute;
    double bestGain;
    bool isNumerical;
    double threshold;
    int attributeIndex;
    
    splitinfo(string attr, double gain, bool isNum, double thresh, int attrIdx)
        : attribute(attr), bestGain(gain), isNumerical(isNum), threshold(thresh), attributeIndex(attrIdx) {}
};


double entropy(const vector<instance>& instances) {
    unordered_map<string, int> classCounts;
    for (const auto& inst : instances) {
        classCounts[inst.classLabel]++;
    }
    
    double totalInstances = instances.size();
    double ent = 0.0;
    
    for (const auto& pair : classCounts) {
        double prob = pair.second / totalInstances;
        ent -= prob * log2(prob);
    }
    
    return ent;
}


double informationGain(const vector<instance>& instances, const string& attribute) {
    unordered_map<string, vector<instance>> subsets;
    
    for (const auto& inst : instances) {
        subsets[inst.attributes.at(attribute)].push_back(inst);
    }
    
    double totalInstances = instances.size();
    double baseEntropy = entropy(instances);
    double weightedEntropy = 0.0;
    
    for (const auto& pair : subsets) {
        double subsetSize = pair.second.size();
        if (subsetSize == 0) continue;
        double wt_avg = subsetSize / totalInstances;
        weightedEntropy += wt_avg * entropy(pair.second);
    }
    
    return baseEntropy - weightedEntropy;
}

double intrinsicValue(const vector<instance>& instances, const string& attribute) {
    unordered_map<string, int> valueCounts;
    
    for (const auto& inst : instances) {
        valueCounts[inst.attributes.at(attribute)]++;
    }
    
    double totalInstances = instances.size();
    double intrinsic = 0.0;
    
    for (const auto& pair : valueCounts) {
        double prob = pair.second / totalInstances;
        intrinsic -= prob * log2(prob);
    }
    
    return intrinsic;
}

double gainRatio(const vector<instance>& instances, const string& attribute) {
    double gain = informationGain(instances, attribute);
    double intrinsic = intrinsicValue(instances, attribute);
    
    assert(intrinsic != 0.0);
    return gain / intrinsic;
}

double NWIG(const vector<instance>& instances, const string& attribute) {
    double gain = informationGain(instances, attribute);
    unordered_map<string, int> valueCounts;
    for(const auto& inst : instances) {
       auto v = inst.attributes.at(attribute);
       valueCounts[v]++;
    }
    int k = valueCounts.size();
    int totalInstances = instances.size();
  
    return (gain / log2(k + 1))*(1 - (k-1.0)/totalInstances);
}

splitinfo findBestSplit(const vector<instance>& instances, const Dataset& dataset, const string& criterion) {
    splitinfo bestsplit("",0.0, false, 0.0, -1);

    assert(!instances.empty() && !dataset.attributeNames.empty());
    assert(criterion == "IG" || criterion == "IGR" || criterion == "NWIG");

    for(int i =0; i<dataset.attributeNames.size(); i++) {
        double gain  = 0;
        const string& attribute = dataset.attributeNames[i];
        if(criterion=="IG"){
            gain = informationGain(instances, attribute);
        } else if(criterion=="IGR") {
            gain = gainRatio(instances, attribute);
        } else if(criterion=="NWIG") {
            gain = NWIG(instances, attribute);
        }
        if (gain > bestsplit.bestGain) {
            bestsplit = splitinfo(attribute, gain, false, 0.0, i);
        }
    }
    return bestsplit;
}


string majorityClass(const vector<instance>& instances) {
    unordered_map<string, int> classCounts;
    for (const auto& inst : instances) {
        classCounts[inst.classLabel]++;
    }
    
    string majorityClassLabel;
    int maxCount = 0;
    
    for (const auto& pair : classCounts) {
        if (pair.second > maxCount) {
            maxCount = pair.second;
            majorityClassLabel = pair.first;
        }
    }
    
    return majorityClassLabel;
}

node* buildtree(const vector<instance>& instances, const Dataset& dataset, vector<string>attributes, const string& criterion, int depth ) {
    if (instances.empty()) {
        return nullptr;
    }
    
    string majority = majorityClass(instances);
    node* root = new node(majority, true);
    
    // Check if all instances belong to the same class
    bool allSameClass = true;
    for (const auto& inst : instances) {
        if (inst.classLabel != majority) {
            allSameClass = false;
            break;
        }
    }
    
    if (allSameClass) {
        return root;
    }

    if(MAX_DEPTH > 0 && depth >= MAX_DEPTH || attributes.empty()) {
        root->isLeaf = true; // Mark as leaf node
        root->classLabel = majority; // Assign majority class label
        return root;
    }
    
    
    splitinfo bestSplit = findBestSplit(instances, dataset, criterion);
    
    if (bestSplit.bestGain <= 0 or bestSplit.attributeIndex == -1) {
        return root;
    }
    
    node* new_node = new node();
    new_node->attribute = bestSplit.attribute;
    new_node->attributeIndex = bestSplit.attributeIndex;
    new_node->isLeaf = false;
    root = new_node;

    vector<string> remainingAttributes;
    for (const auto& attr : attributes) {
        if (attr != bestSplit.attribute) {
            remainingAttributes.push_back(attr);
        }
    }
    unordered_map<string, vector<instance>> subsets;
    for (const auto& inst : instances) {
        subsets[inst.attributes.at(bestSplit.attribute)].push_back(inst);
    }
    
    for (const auto& pair : subsets) {
        const string& value = pair.first;
        const vector<instance>& subset = pair.second;

        node* childNode = buildtree(subset, dataset, attributes, criterion, depth + 1);
        childNode->value = value;
        root->children[value] = childNode;
        
        if (childNode->isLeaf) {
            childNode->classLabel = majority;
        } else {
            childNode->isLeaf = false; 
        }
    }
    
    return root;
}

string predict(node* root, const instance& inst) {
    if (root->isLeaf) {
        return root->classLabel;
    }
    
    const string& attributeValue = inst.attributes.at(root->attribute);
    if(root->children.find(attributeValue) == root->children.end()) {
        return root->classLabel;
    }
    node* childNode = root->children[attributeValue];
    return predict(childNode, inst);
}

double accuracy(node* root, const vector<instance>& instances) {
    int correct = 0;
    for (const auto& inst : instances) {
        string predictedLabel = predict(root, inst);
        if (predictedLabel == inst.classLabel) {
            correct++;
        }
    }
    return 1.0*(correct) / instances.size();
}











