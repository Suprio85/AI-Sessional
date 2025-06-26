#include<bits/stdc++.h>
using namespace std;


struct instance {
    unordered_map<string, string> attributes; // Attributes of the instance
    string classLabel; // Class label of the instance
};

struct Dataset{
    string filename;
    int sampleSize;
    vector<string> attributeNames;
    vector<string> classLabels;
    string targetAttribute;
    vector<vector<string>> data;
    vector<instance> instances;
    vector<int> categoricalindices; // Indices of categorical attributes
    vector<int> numericalindices; // Indices of numerical attributes

    Dataset(string filename) : filename(filename){

    }


    void printSummary() {
        cout << "Dataset: " << filename << endl;
        cout << "Number of attributes: " << attributeNames.size() << endl;
        cout << "Number of instances: " << data.size() << endl;
        cout << "Target attribute: " << targetAttribute << endl;
        cout << "Categorical attributes: ";
        for (int idx : categoricalindices) {
            cout << attributeNames[idx] << " ";
        }
        cout << endl;
        cout << "Numerical attributes: ";
        for (int idx : numericalindices) {
            cout << attributeNames[idx] << " ";
        }
        cout << endl;
    }

    void head(int n = 5) {
        if (data.empty()) return;
        cout << "First " << n << " instances:" << endl;
        for (int i = 0; i < min(n, (int)data.size()); ++i) {
            for (const auto& value : data[i]) {
                cout << value << " ";
            }
            cout << endl;
        }
    }
};


Dataset loadData(string filename) {
    Dataset dataset(filename);
    ifstream file(filename);
    string line;

    // Read header
    if (getline(file, line)) {
        stringstream ss(line);
        string attribute;
        while (getline(ss, attribute, ',')) {
            dataset.attributeNames.push_back(attribute);
        }
        dataset.targetAttribute = dataset.attributeNames.back();
    }

    // Read data
    while (getline(file, line)) {
        vector<string> row;
        stringstream ss(line);
        string value;
        while (getline(ss, value, ',')) {
            row.push_back(value);
        }
        if (!row.empty()) {
            dataset.data.push_back(row);
            instance inst;
            for (size_t i = 0; i < row.size(); ++i) {
                inst.attributes[dataset.attributeNames[i]] = row[i];
            }
            inst.classLabel = row.back();
            dataset.instances.push_back(inst);
        }
    }

    // Identify categorical and numerical attributes
    for (size_t i = 0; i < dataset.attributeNames.size(); ++i) {
        bool isCategorical = false;
        for (const auto& row : dataset.data) {
            if (!isdigit(row[i][0])) {
                isCategorical = true;
                break;
            }
        }
        if (isCategorical) {
            dataset.categoricalindices.push_back(i);
        } else {
            dataset.numericalindices.push_back(i);
        }
    }

    file.close();
    return dataset;
}

// split data into training and testing sets
pair<Dataset, Dataset> trainTestSplit(const Dataset& dataset, double trainRatio = 0.8) {
    Dataset trainSet(dataset.filename);
    Dataset testSet(dataset.filename);
    
    trainSet.attributeNames = dataset.attributeNames;
    trainSet.classLabels = dataset.classLabels;
    trainSet.targetAttribute = dataset.targetAttribute;
    trainSet.categoricalindices = dataset.categoricalindices;
    trainSet.numericalindices = dataset.numericalindices;
    
    testSet.attributeNames = dataset.attributeNames;
    testSet.classLabels = dataset.classLabels;
    testSet.targetAttribute = dataset.targetAttribute;
    testSet.categoricalindices = dataset.categoricalindices;
    testSet.numericalindices = dataset.numericalindices;

    int trainSize = static_cast<int>(dataset.data.size() * trainRatio);

    // Shuffle the dataset
    vector<int> indices(dataset.data.size());
    iota(indices.begin(), indices.end(), 0);
    shuffle(indices.begin(), indices.end(), default_random_engine(random_device{}()));
    // Split the dataset
    
    for (int i = 0; i < trainSize; ++i) {
        trainSet.data.push_back(dataset.data[indices[i]]);
        trainSet.instances.push_back(dataset.instances[indices[i]]);
    }

    trainSet.sampleSize = trainSet.data.size();
    
    for (int i = trainSize; i < dataset.data.size(); ++i) {
        testSet.data.push_back(dataset.data[indices[i]]);
        testSet.instances.push_back(dataset.instances[indices[i]]);
    }
    testSet.sampleSize = testSet.data.size();
    
    return make_pair(trainSet, testSet);
}



    

