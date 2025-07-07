#include<bits/stdc++.h>
using namespace std;

string trim(const string& str) {
    int first_pos = -1;
    int last_pos = -1;
    for(int i=0; i<str.size(); i++){
        if(str[i]!=' '){
            first_pos = i;
            break;
        }
    }
     for(int i=str.size()-1; i>=0; i--){
        if(str[i]!= ' '){
            last_pos = i;
            break;
        }
    }

    return str.substr(first_pos,last_pos-first_pos+1);
}

struct instance {
    unordered_map<string, string> attributes;
    string classLabel;
};

struct Dataset{
    string filename;
    int sampleSize;
    vector<string> attributeNames;
    vector<string> classLabels;
    string targetAttribute;
    vector<vector<string>> data;
    vector<instance> instances;
    vector<int> categoricalindices; //categorical attributes
    vector<int> numericalindices; //  numerica attributes

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
        for (int i = 0; i < min(n, (int)data.size()); i++) {
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
        vector<string> header;
        while (getline(ss, attribute, ',')) {
            attribute = trim(attribute);
            header.push_back(attribute);
        }
        for(int i = 0; i < header.size()-1; ++i) {
            dataset.attributeNames.push_back(header[i]);
        }
        dataset.targetAttribute = header.back();

    }

    
    while (getline(file, line)) {
        vector<string> row;
        stringstream ss(line);
        string value;
        while (getline(ss, value, ',')) {
            value = trim(value);
            row.push_back(value);
        }
        if (!row.empty()) {
            dataset.data.push_back(row);
            instance inst;
            for (int i = 0; i < row.size()-1; ++i) {
                inst.attributes[dataset.attributeNames[i]] = row[i];
            }
            inst.classLabel = row.back();
            dataset.instances.push_back(inst);
        }
    }

  
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
void imputeMissingValues(Dataset& dataset) {
    unordered_map<string, unordered_map<string, int>> frequencyMap;

    for (const auto& inst : dataset.instances) {
        for (const auto& attr : dataset.attributeNames) {
            const string& val = inst.attributes.at(attr);
            if (val != "?") {
                frequencyMap[attr][val]++;
            }
        }
    }

    unordered_map<string, string> attrMode;
    for (const auto& [attr, valueCount] : frequencyMap) {
        string modeValue;
        int maxCount = 0;
        for (const auto& [val, count] : valueCount) {
            if (count > maxCount) {
                maxCount = count;
                modeValue = val;
            }
        }
        attrMode[attr] = modeValue;
    }

    for (auto& inst : dataset.instances) {
        for (auto& [attr, val] : inst.attributes) {
            if (val == "?") {
                val = attrMode[attr];
            }
        }
    }

}

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

   
    vector<int> indices(dataset.data.size());
    iota(indices.begin(), indices.end(), 0);
    shuffle(indices.begin(), indices.end(), default_random_engine(random_device{}()));
    
    
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



    

