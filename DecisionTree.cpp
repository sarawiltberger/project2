#include "DecisionTree.h"

using namespace std;

DecisionTree::DecisionTree(string _inputFile, int _maxDepth, bool train){
	inputFile = _inputFile;
	maxDepth = _maxDepth;
	dataVector = readData();
    numAttributes = featureVector.size()-1;
	//randomly shuffle dataVector
	random_shuffle (dataVector.begin(), dataVector.end());
    if(train){
        int testSetStartIndex = dataVector.size()*.8;
        int validSetEndIndex = testSetStartIndex -1;
        int validSetStartIndex = dataVector.size()*.6;
        int trainSetEndIndex = validSetStartIndex-1;
        vector<vector<string>> trainData(dataVector.begin(), dataVector.begin()+trainSetEndIndex);
        vector<vector<string>> validData(dataVector.begin()+(validSetStartIndex), dataVector.begin()+validSetEndIndex);
        vector<vector<string>> testData(dataVector.begin()+(testSetStartIndex), dataVector.end());
        auto node= make_shared<Node>();
        root = buildTree(node, trainData, trainData);
        trainAcc = accuracy(root, validData);
        test = accuracy(root, testData);
        
    }else{
        int testSetStartIndex = dataVector.size()*.8;
        int trainSetEndIndex = testSetStartIndex-1;
        vector<vector<string>> trainData(dataVector.begin(), dataVector.begin()+trainSetEndIndex);
        vector<vector<string>> testData(dataVector.begin()+testSetStartIndex, dataVector.end());
         auto node= make_shared<Node>();
        root = buildTree(node, trainData,trainData);
        //root = buildTree(node, dataVector, dataVector);
        test = accuracy(root, testData);
    }
	
    

}

void DecisionTree::printTree(){
    printTree(root);
}

double DecisionTree::getTrainAccuracy(){
    return trainAcc;
}
double DecisionTree::getTestAccuracy(){
    return test;
}

void DecisionTree::printTree(shared_ptr<Node> node){
	if(node == NULL){return;}
	if (!node->children.empty()) {

		cout << "Split on: " << node->feature <<endl;
		for (int i = 0; i < node->children.size(); i++) {
			cout << "\t";
            cout <<node->childrenVals[i] << " ";
			printTree(node->children[i]);
		}
		return;
	}else{
        cout << "Class: " << node->value <<  "Depth: " << node->depth << endl;
		return;
	}

}

string DecisionTree::TreeOutput(shared_ptr<DecisionTree::Node> node,  vector<string>& testExample){
    string output = node->value;
    while(!node->isLeaf && !node->children.empty()){
        int index = featureToIndex[node->feature];
        string value = testExample[index];
        int childIndex = -1;
        for(int j = 0; j < featurePossibleValues[index].size(); j++){
            if(featurePossibleValues[index][j] == value){
                childIndex = j;
                break;
            }
        }
        if(childIndex == -1){
            cout << "here bad" << endl;
        }
        node = node->children[childIndex];
        output = node->value;

    }
    return output;
}


double DecisionTree::accuracy(shared_ptr<DecisionTree::Node> tree,  vector<vector<string>>& testTable){
    int numRight = 0;
    for(int i = 0; i < testTable.size(); i++){
        //find the tree output
        string treeOutput = TreeOutput(tree, testTable[i]);
        if(testTable[i][0] == treeOutput){
            numRight++;
        }
        /*if(tree->isLeaf){
            cout << tree->value << ",";
        }*/
    }
    
    return (double)numRight/testTable.size();
}

shared_ptr<DecisionTree::Node> DecisionTree::buildTree(shared_ptr<Node> currNode, vector<vector<string>>& examples, vector<vector<string>>& parentExamples){
    //cout << "size: " << examples.size()<<endl;
    if(examples.size() ==0){
        currNode -> value = mostCommonClass(parentExamples);
        currNode ->isLeaf = true;
        return currNode;
    }
    //if no attributes
    if(numAttributes <=0){
        currNode -> value = mostCommonClass(examples);
        currNode ->isLeaf = true;
        return currNode;
    }
    if(currNode->depth == maxDepth){
        currNode->isLeaf = true;

        currNode->value = mostCommonClass(examples); //class is the first one
        return currNode;
        
    }
    if(allSameClass(examples)){
        //int fIndex = featureToIndex(currNode->feature);
        currNode->isLeaf = true;
        currNode->value = examples[0][0]; //class is the first one
        return currNode;
    }else{
        string attributeToSplit = getSplitAttribute(examples);
        //cout << "attribute to split: " <<attributeToSplit << endl;
        currNode->feature = attributeToSplit;
        int featureIndex = featureToIndex[attributeToSplit];
        featureVector[featureIndex] = "-1";
        numAttributes--;
        //cout << "feature index: " <<featureIndex << endl;
        //cout << "table size " <<examples.size() << endl;
        //each branch for tree
        for(int i = 0; i < featurePossibleValues[featureIndex].size(); i++){
            auto childNode= make_shared<Node>();
            //childNode->value = featurePossibleValues[featureIndex][i];
            //cout << "child node value: " <<childNode->value << endl;
            childNode->depth = currNode->depth +1;
            currNode->childrenVals.push_back(featurePossibleValues[featureIndex][i]);
            childNode->isLeaf = false;
            //childNode->feature = attributeToSplit;
            //cout << "prune on: " << featurePossibleValues[featureIndex][i] <<endl;
            vector<vector<string>> childExamples = pruneTable(featurePossibleValues[featureIndex][i], featureIndex, examples);
            currNode->children.push_back(buildTree(childNode, childExamples, examples));
        }
    }
    return currNode;
    
}




/*shared_ptr<DecisionTree::Node> DecisionTree::buildTree(shared_ptr<Node> currNode, vector<vector<string>>& dataTable){
    if(dataTable.size() ==0){
        return NULL;
    }
    if(dataTable.size() == 1){
        currNode->isLeaf = true;
        currNode->value = dataTable[0][0]; //class is the first one
        return currNode;
	}
    if(currNode->depth == maxDepth){
        currNode->isLeaf = true;
        currNode->value = mostCommonClass(dataTable); //class is the first one
        return currNode;
        
    }
	if(allSameClass(dataTable)){
        //int fIndex = featureToIndex(currNode->feature);
		currNode->isLeaf = true;
		currNode->value = dataTable[0][0]; //class is the first one
		return currNode;
	}else{
		string attributeToSplit = getSplitAttribute(dataTable);
        cout << "attribute to split: " <<attributeToSplit << endl;
		currNode->feature = attributeToSplit;
		int featureIndex = featureToIndex[attributeToSplit];
        cout << "feature index: " <<featureIndex << endl;
        cout << "table size " <<dataTable.size() << endl;
		for(int i = 0; i < featurePossibleValues[featureIndex].size(); i++){
			shared_ptr<Node> childNode(new Node);
			childNode->value = featurePossibleValues[featureIndex][i];
            //cout << "child node value: " <<childNode->value << endl;
			childNode->depth = currNode->depth +1;
			currNode->childrenVals.push_back(featurePossibleValues[featureIndex][i]);
			childNode->isLeaf = false;
			childNode->feature = attributeToSplit;
			vector<vector<string>> prunedTable = pruneTable(featurePossibleValues[featureIndex][i], featureIndex, dataTable);
			currNode->children.push_back(buildTree(childNode, prunedTable));
		}
	}
	return currNode;

}
*/
string DecisionTree::mostCommonClass(vector<vector<string>>& dataTable){
    int num0 = 0;
    int num1 = 0;
    for(int i = 0; i < dataTable.size(); i++){
        if(dataTable[i][0] == "0"){
            num0++;
        }else{
            num1++;
        }
    }
    if(num0 > num1) return "0";
    return "1";
}

vector<vector<string>> DecisionTree::pruneTable(string val, int featureIndex, vector<vector<string>>& dataTable){
	vector<vector<string>> prunedTable;
	for(int i = 0; i < dataTable.size(); i++){
		if(dataTable[i][featureIndex] == val){
			prunedTable.push_back(dataTable[i]);
		}
	}
	return prunedTable;

}
vector<int> DecisionTree::numFeatureValues(int featureIndex, vector<vector<string>>& dataTable){
//puts the sum in the back
	vector<int> counts(featurePossibleValues[featureIndex].size());
    fill (counts.begin(),counts.end(), 0);
	for(int i = 0; i < dataTable.size(); i++){
        for(int j = 0; j < featurePossibleValues[featureIndex].size(); j++){
            if(dataTable[i][featureIndex] == featurePossibleValues[featureIndex][j]){
                counts[j]++;
            }
        }
    }
	int sum = 0;
	for(int i = 0; i < counts.size(); i ++){
		sum += counts[i];
	}
	counts.push_back(sum);
	return counts;
}

string DecisionTree::getSplitAttribute(vector<vector<string>>& dataTable){
    double minEntropy = DBL_MAX;
    int splitCol = -1;
    //for each feature, not including the class
    for(int col = 1; col < dataTable[0].size(); col++){
        if(featureVector[col] == "-1"){
            continue;
        }
        //for each possible feature value, get a count of how many there are
        vector<int> countFeatureValues = numFeatureValues(col, dataTable);
        //for each possible feature
        double featureEntropy = 0.0;
        //cout << "feature " << featureVector[col]<<endl;
        for(int fv = 0; fv < featurePossibleValues[col].size(); fv++){
            string featureValue = featurePossibleValues[col][fv];
            //need to know how many are positive
            int numFeatureValuePositve = numValuePositive(col, featureValue, dataTable);
            double entropyValPosRatio;
            double entropyValPos;
            double entropyValNegRatio;
            double entropyValNeg;
            if(countFeatureValues[fv] == 0){
                continue;
            }else{
                entropyValPosRatio = ((double)numFeatureValuePositve/countFeatureValues[fv]);
                if(entropyValPosRatio == 0){
                    entropyValPos = 0;
                }else if(entropyValPosRatio == 1){
                    entropyValPos = 0;
                }else{
                    entropyValPos= entropyValPosRatio*(log(entropyValPosRatio)/log(2));
                }
                entropyValNegRatio = ((double)(countFeatureValues[fv]-numFeatureValuePositve)/countFeatureValues[fv]);
                if(entropyValNegRatio == 0){
                    entropyValNeg = 0;
                }else if(entropyValNegRatio == 1){
                    entropyValNeg = 0;
                }else{
                    entropyValNeg= entropyValNegRatio*(log(entropyValNegRatio)/log(2));
                }
            }
            
            double featureValEntropy = entropyValPos + entropyValNeg;

            //cout << "value entropy " << featureValEntropy <<endl;
            //number with this feature value / all examples
            double featureRatio = (double)countFeatureValues[fv] / (double)countFeatureValues[countFeatureValues.size()-1];
            featureEntropy -= (featureRatio)*featureValEntropy;
        }
        //cout << "feature entropy " << featureEntropy <<endl;
        //cout << "FEATURE ENTROPY": featureEntropy << endl;
        if(featureEntropy < minEntropy){
            minEntropy = featureEntropy;
            splitCol = col;
        }
        
    }
    return featureVector[splitCol];
    
}

int DecisionTree::numValuePositive(int featureIndex, string value, vector<vector<string>>& dataTable){
    //returns count of how many examples where feature = value and class = 1
    int numMatches = 0;
    for(int i = 0; i < dataTable.size(); i++){
        if(dataTable[i][featureIndex] == value && dataTable[i][0] == "1"){
            numMatches++;
        }
    }
    return numMatches;
}

/*
string DecisionTree::getSplitAttribute(vector<vector<string>>& dataTable){
	double minEntropy = DBL_MAX;
	int splitCol = -1;
    //for each feature, not including the class
	for(int col = 1; col < dataTable[0].size(); col++){
        //for each possible feature value, get a count of how many there are
		vector<int> countFeatureValues = numFeatureValues(col, dataTable);
		vector<double> featureEntropy;
		double colEntropy = 0.0;
		map<string, int> valFrequency;
		for(int row = 0; row < dataTable.size(); row++){
			double entropy = 0.0;
			if(valFrequency.find(dataTable[row][col]) != valFrequency.end()){
				valFrequency[dataTable[row][col]]++;
			}else{
				valFrequency[dataTable[row][col]] =1;
				vector<vector<string>> tableWithFeatureVal = pruneTable(dataTable[row][col], col, dataTable);
				vector<int> numDiffClasses = numFeatureValues(0, tableWithFeatureVal);
				for(int i = 0; i < numDiffClasses.size()-1; i++){
					double currValCount = numDiffClasses[i];
					entropy -= (currValCount/numDiffClasses[numDiffClasses.size()-1])*(log(currValCount/numDiffClasses[numDiffClasses.size()-1]) / log(2));
				}
				featureEntropy.push_back(entropy);
				entropy = 0.0;
			}
		}
		for(int i = 0; i < countFeatureValues.size()-1; i++){
			colEntropy += ((double) countFeatureValues[i] * (double) featureEntropy[i]);
		}
		colEntropy = colEntropy / ((double) countFeatureValues[countFeatureValues.size()-1]);
		if(colEntropy <= minEntropy){
			minEntropy = colEntropy;
			splitCol = col;
		}

	}
	return featureVector[splitCol];

}*/

bool DecisionTree::allSameClass(vector<vector<string>>& dataTable){
    string firstVal = dataTable[0][0];
	for(int i = 1; i < dataTable.size(); i++){
		if(firstVal != dataTable[i][0]){
			return false;
		}
	}
	return true;

}

vector<string> DecisionTree::parseDataString(string str, bool notFirst){
	vector<string> vectorOfSections;
	int i = 0;
	while (str.length() != 0 && str.find(',') != string::npos){
		size_t pos;
		string section;
		pos = str.find_first_of(',');
		section = str.substr(0, pos);
		vectorOfSections.push_back(section);
		if(notFirst){
			featurePossibleValues[i];
			if(find(featurePossibleValues[i].begin(), featurePossibleValues[i].end(), section) == featurePossibleValues[i].end()) {
			    featurePossibleValues[i].push_back(section);
			}
		}else{
			featureToIndex[section] = i;
			featurePossibleValues.push_back(vector<string>());
		}
		str.erase(0, pos+1);
		i++;
	}
    vectorOfSections.push_back(str);
    if(notFirst){
        featurePossibleValues[i];
        if(find(featurePossibleValues[i].begin(), featurePossibleValues[i].end(), str) == featurePossibleValues[i].end()) {
            featurePossibleValues[i].push_back(str);
        }
    }else{
        featureToIndex[str] = i;
        featurePossibleValues.push_back(vector<string>());
    }
	return vectorOfSections;
}

void DecisionTree::print2dVect(vector<std::vector<string> > vec){ //for testing
	for (int i = 0; i < vec.size(); i++){
	    for (int j = 0; j < vec[i].size(); j++){
	        cout << vec[i][j] << " ";
	    }
			cout << endl;
	}
}

vector<vector<string>> DecisionTree::readData(){
    vector<vector<string>> dataTable;
    ifstream in;
	in.open(inputFile);
    if (!in){
        cout << "error opening input file\n";
        return dataTable;
    }
	string firstLine;
	getline(in, firstLine);
	featureVector = parseDataString(firstLine, false);
	string oneLine;
	while (getline(in, oneLine)){
		vector<string> row = parseDataString(oneLine, true);
		dataTable.push_back(row);
	}
	in.close();
    //print2dVect(featurePossibleValues);
    return dataTable;

}


