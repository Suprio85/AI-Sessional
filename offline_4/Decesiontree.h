#include <bits/stdc++.h>
#include "Dataloader.hpp"
#include "chrono"
using namespace std;
using namespace std::chrono;

#define MAX_DEPTH 5

struct node
{
    string attribute;
    int attributeIndex;
    string value;
    unordered_map<string, node *> children;
    double threshold;
    string classLabel;
    bool isLeaf;
    bool isNumerical;
    node *left = nullptr;
    node *right = nullptr;

    node() : isLeaf(false), threshold(0.0), attributeIndex(-1)
    {
        left = nullptr;
        right = nullptr;
    }
    node(string classLabel, bool isLeafNode = true) : classLabel(classLabel), isLeaf(isLeafNode)
    {
        left = nullptr;
        right = nullptr;
    }
    ~node()
    {
        for (auto &child : children)
        {
            delete child.second;
        }
        children.clear();
        delete left;
        delete right;
    }
};

struct splitinfo
{
    string attribute;
    double bestGain;
    bool isNumerical;
    double threshold;
    int attributeIndex;
   

    splitinfo(string attr, double gain, bool isNum, double thresh, int attrIdx)
        : attribute(attr), bestGain(gain), isNumerical(isNum), threshold(thresh), attributeIndex(attrIdx) {}
};

int countNodes(const node* n) {
    if (!n) return 0;
    int cnt = 1;
    for (auto &p : n->children) cnt += countNodes(p.second);
    cnt += countNodes(n->left);
    cnt += countNodes(n->right);
    return cnt;
}


int calculateTreeDepth(const node* n) {
    if (!n || n->isLeaf) return 0;
    int depthCat = 0;
    for (auto &p : n->children)
        depthCat = max(depthCat, calculateTreeDepth(p.second));
    int depthNum = max(calculateTreeDepth(n->left), calculateTreeDepth(n->right));
    return 1 + max(depthCat, depthNum);
}


void printTree(const node* n, const string& indent = "") {
    if (n == nullptr) {
        cout << indent << "Null node" << endl;
        return;
    }
    if (n->isLeaf) {
        cout << indent << "Class: " << n->classLabel << endl;
        return;
    }
    
    if (n->isNumerical) {
        cout << indent << "[Numeric] " << n->attribute 
             << " <= " << n->threshold << endl;
        cout << indent << "  True branch:" << endl;
        printTree(n->left, indent + "    ");
        cout << indent << "  False branch:" << endl;
        printTree(n->right, indent + "    ");
    } else {
        cout << indent << "[Categorical] " << n->attribute << endl;
        for (const auto& child : n->children) {
            cout << indent << "  Value '" << child.first << "':" << endl;
            printTree(child.second, indent + "    ");
        }
    }
}


double entropy(const vector<instance> &instances)
{
    unordered_map<string, int> classCounts;
    for (const auto &inst : instances)
    {
        classCounts[inst.classLabel]++;
    }

    double totalInstances = instances.size();
    double ent = 0.0;

    for (const auto &pair : classCounts)
    {
        double prob = pair.second / totalInstances;
        ent -= prob * log2(prob);
    }

    return ent;
}

double informationGain(const vector<instance> &instances, const string &attribute)
{
    unordered_map<string, vector<instance>> subsets;

    for (const auto &inst : instances)
    {
        subsets[string(inst.attributes.at(attribute))].push_back(inst);
    }

    double totalInstances = instances.size();
    double baseEntropy = entropy(instances);
    double weightedEntropy = 0.0;

    for (const auto &pair : subsets)
    {
        double subsetSize = pair.second.size();
        if (subsetSize == 0)
            continue;
        double wt_avg = subsetSize / totalInstances;
        weightedEntropy += wt_avg * entropy(pair.second);
    }
   

    return baseEntropy - weightedEntropy;
}

double intrinsicValue(const vector<instance> &instances, const string &attribute)
{
    unordered_map<string, int> valueCounts;

    for (const auto &inst : instances)
    {
        valueCounts[string(inst.attributes.at(attribute))]++;
    }
   
   
    double totalInstances = instances.size();
    double intrinsic = 0.0;

    for (const auto &pair : valueCounts)
    {
        double prob = pair.second / totalInstances;
        intrinsic -= prob * log2(prob);
          
    }

    return intrinsic;
}

double gainRatio(const vector<instance> &instances, const string &attribute)
{
    double gain = informationGain(instances, attribute);
    double intrinsic = intrinsicValue(instances, attribute);

    return intrinsic != 0.0 ? gain / intrinsic : 0.0;
}



double NWIG(const vector<instance> &instances, const string &attribute)
{
    double gain = informationGain(instances, attribute);
    unordered_map<string, int> valueCounts;
    for (const auto &inst : instances)
    {
        auto v = inst.attributes.at(attribute);
        valueCounts[string(v)]++;
    }
    int k = valueCounts.size();
    int totalInstances = instances.size();

    return (gain / log2(k + 1)) * (1 - (k - 1.0) / totalInstances);
}

pair<double, double> findbestThreshold(const vector<instance> &instances, const string &attribute, const string &criterion)
{
    // auto start_total = high_resolution_clock::now();
    vector<pair<double, string>> sortedValues;
    for (const auto &inst : instances)
        sortedValues.emplace_back(stod(inst.attributes.at(attribute)), inst.classLabel);
    
    sort(sortedValues.begin(), sortedValues.end());

    // auto end_sort = high_resolution_clock::now();
    // cout << "[Time] Sort & preparation: " << duration<double>(end_sort - start_total).count() << " sec" << endl;

    unordered_map<string, int> totalCounts;
    for (const auto &p : sortedValues)
        totalCounts[p.second]++;

    unordered_map<string, int> leftCounts, rightCounts = totalCounts;

    double bestGain = -1.0;
    double bestThreshold = -1.0;
    int totalSize = sortedValues.size();

    auto start_sweep = high_resolution_clock::now();

    for (int i = 1; i < totalSize; ++i)
    {
        const string &cls = sortedValues[i - 1].second;
        leftCounts[cls]++;
        rightCounts[cls]--;

       
        if (sortedValues[i - 1].first == sortedValues[i].first)
            continue;

        double threshold = (sortedValues[i - 1].first + sortedValues[i].first) / 2.0;

       
        int leftSize = i;
        int rightSize = totalSize - i;

        double leftEntropy = 0.0, rightEntropy = 0.0;

        for (const auto &[label, count] : leftCounts)
        {
            double p = 1.0 * count / leftSize;
            leftEntropy -= p * log2(p);
        }

        for (const auto &[label, count] : rightCounts)
        {
            if (count == 0) continue;
            double p = 1.0 * count / rightSize;
            rightEntropy -= p * log2(p);
        }

        double totalEntropy = (leftSize * leftEntropy + rightSize * rightEntropy) / totalSize;
        double baseEntropy = 0.0;

        for (const auto &[label, count] : totalCounts)
        {
            double p = 1.0 * count / totalSize;
            baseEntropy -= p * log2(p);
        }

        double gain = baseEntropy - totalEntropy;

        if (criterion == "IGR")
        {
            double leftRatio = 1.0 * leftSize / totalSize;
            double rightRatio = 1.0 * rightSize / totalSize;
            double intrinsic = -(leftRatio * log2(leftRatio) + rightRatio * log2(rightRatio));
            if (intrinsic > 0)
                gain /= intrinsic;
            else
                gain = 0;
        }
        else if (criterion == "NWIG")
        {
            int k = 2;
            gain = (gain / log2(k + 1)) * (1 - (k - 1.0) / totalSize);
        }

        if (gain > bestGain)
        {
            bestGain = gain;
            bestThreshold = threshold;
        }
    }

    // auto end_sweep = high_resolution_clock::now();
    // auto end_total = high_resolution_clock::now();

    // cout << "[Time] Sweep: " << duration<double>(end_sweep - start_sweep).count() << " sec" << endl;
    // cout << "[Time] Total: " << duration<double>(end_total - start_total).count() << " sec" << endl;

    return {bestGain, bestThreshold};
}

splitinfo findBestSplit(const vector<instance>& instances, const Dataset& dataset, const vector<int>& attributes, const string& criterion) {
    splitinfo bestsplit("",0.0, false, 0.0, -1);

    assert(!instances.empty() && !attributes.empty());
    assert(criterion == "IG" || criterion == "IGR" || criterion == "NWIG");

    for (int attrIndex : attributes) {
        const string& attribute = dataset.attributeNames[attrIndex];
        bool isNumerical = find(dataset.numericalindices.begin(), dataset.numericalindices.end(), attrIndex) != dataset.numericalindices.end();
        
        if (isNumerical) {
            auto [gain, threshold] = findbestThreshold(instances, attribute, criterion);
            if (gain > bestsplit.bestGain) {
                bestsplit = splitinfo(attribute, gain, true, threshold, attrIndex);
            }
        } else {
            double gain;
            if(criterion == "IG") {
                gain = informationGain(instances, attribute);
            } else if(criterion == "IGR") {
                gain = gainRatio(instances, attribute);
            } else if(criterion == "NWIG") {
                gain = NWIG(instances, attribute);
            }
            
            if (gain > bestsplit.bestGain) {
                bestsplit = splitinfo(attribute, gain, false, 0.0, attrIndex);
            }
        }
    }
    return bestsplit;
}

bool allSameClass(const vector<instance>& instances) {
    if (instances.empty()) return true;

    const string& firstLabel = instances.front().classLabel;
    for (const auto& inst : instances) {
        if (inst.classLabel != firstLabel)
            return false;
    }
    return true;
}


string majorityClass(const vector<instance> &instances)
{
    unordered_map<string, int> classCounts;
    for (const auto &inst : instances)
    {
        classCounts[inst.classLabel]++;
    }

    string majorityClassLabel;
    int maxCount = 0;

    for (const auto &pair : classCounts)
    {
        if (pair.second > maxCount)
        {
            maxCount = pair.second;
            majorityClassLabel = pair.first;
        }
    }

    return majorityClassLabel;
}


node *buildtree(const vector<instance> &instances, const Dataset &dataset, vector<int> attributes, const string &criterion, int depth, int max_depth)
{
    // cout << "Building tree at depth: " << depth << ", instances: " << instances.size() << endl;

    if (instances.empty())
        return nullptr;

    string majority = majorityClass(instances);
    node *root = new node(majority, true);

   
    bool isallSameClass = allSameClass(instances);

    if (isallSameClass || (max_depth > 0 && depth >= max_depth) || attributes.empty())
    {
        return root;
    }

    splitinfo bestSplit = findBestSplit(instances, dataset, attributes, criterion);

    if (bestSplit.bestGain <= 1e-6 || bestSplit.attributeIndex == -1)
    {
        return root;
    }

    node *new_node = new node();
    new_node->attribute = bestSplit.attribute;
    new_node->attributeIndex = bestSplit.attributeIndex;
    new_node->isLeaf = false;
    new_node->isNumerical = bestSplit.isNumerical;
    root = new_node;

    if (root->isNumerical)
    {
        vector<instance> leftSubset, rightSubset;
        for (const auto &inst : instances)
        {
            double val = stod(inst.attributes.at(bestSplit.attribute));
            if (val <= bestSplit.threshold)
                leftSubset.push_back(inst);
            else
                rightSubset.push_back(inst);
        }


        root->threshold = bestSplit.threshold;
        root->left = buildtree(leftSubset, dataset, attributes, criterion, depth + 1, max_depth);
        root->right = buildtree(rightSubset, dataset, attributes, criterion, depth + 1, max_depth);
    }
    else
    {
        vector<int> remainingAttributes;
        for (int attr : attributes)
        {
            if (attr != bestSplit.attributeIndex)
                remainingAttributes.push_back(attr);
        }

        unordered_map<string, vector<instance>> subsets;
        for (const auto &inst : instances)
        {
            subsets[inst.attributes.at(bestSplit.attribute)].push_back(inst);
        }

        for (const auto &pair : subsets)
        {
            const string &value = pair.first;
            const vector<instance> &subset = pair.second;

            node *childNode = buildtree(subset, dataset, remainingAttributes, criterion, depth + 1, max_depth);
            if (childNode)
            {
                childNode->value = value;
                root->children[value] = childNode;
            }
        }
    }

    return root;
}


string predict(node *root, const instance &inst)
{
    if (root->isLeaf)
    {
        return root->classLabel;
    }

    if (root->isNumerical)
    {
       
        double value = stod(inst.attributes.at(root->attribute));
        if (value <= root->threshold)
        {
            return root->left ? predict(root->left, inst) : root->classLabel;
        }
        else
        {
            return root->right ? predict(root->right, inst) : root->classLabel;
        }
    }
    else
    {
        
        const string &attributeValue = inst.attributes.at(root->attribute);
        if (root->children.find(attributeValue) == root->children.end())
        {
            return root->classLabel;
        }
        node *childNode = root->children[attributeValue];
        return predict(childNode, inst);
    }
}

double accuracy(node *root, const vector<instance> &instances)
{
    int correct = 0;
    for (const auto &inst : instances)
    {
        string predictedLabel = predict(root, inst);
        if (predictedLabel == inst.classLabel)
        {
            correct++;
        }
    }
    return 1.0 * (correct) / instances.size();
}
