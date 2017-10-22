#ifndef DECISION_TREE_H
#define DECISION_TREE_H

#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <iomanip>
#include <vector>
#include <map>
#include <math.h>
#include <tgmath.h>
#include <float.h>
#include <memory>
#include <climits>
#include <algorithm>

using namespace std;


class DecisionTree {
public:
	DecisionTree(string _inputFile, int _maxDepth, bool train=true);
	~DecisionTree(){}
  struct Node{
    string feature; //feature you are splitting on
    string value;
    bool isLeaf;
    vector<shared_ptr<Node>> children;
    vector<string> childrenVals;
    int depth =1;

  };
  double accuracy(shared_ptr<Node> tree,  vector<vector<string>>& testTable);
  double getTrainAccuracy();
  double getTestAccuracy();
    void printTree();


private:

  
    
  string inputFile;
  int maxDepth;
  double trainAcc=1;
  double test;
  vector<vector <string > > dataVector; //all data
  vector<string> featureVector;
  vector< vector<string> > featurePossibleValues; //for the feature at i, possible values it could have
  //vector<vector<string>> nonTestData; //what we will train on (80% of data)
  //vector<vector<string>> testData;
  map<string, int> featureToIndex; //maps the feature to the index in the feature, featurePossibleValues, and data vectors
  shared_ptr<Node> root;
  int numAttributes;
  vector<vector<string>> readData();
  vector<string> parseDataString(string str, bool notFirst);
    shared_ptr<Node> buildTree(shared_ptr<Node> currNode, vector<vector<string>>& examples, vector<vector<string>>& parentExamples);
  void print2dVect(vector<std::vector<string> > vec);
  bool allSameClass(vector<vector<string>>& dataTable);
  string getSplitAttribute(vector<vector<string>>& dataTable);
  vector<vector<string>> pruneTable(string val, int featureIndex, vector<vector<string>>& dataTable); // deletes all rows from table with that feature with that value
  vector<int> numFeatureValues(int featureIndex, vector<vector<string>>& dataTable); //how many of each feature value in dataTable (and sums in last)
    int numValuePositive(int featureIndex, string value, vector<vector<string>>& dataTable); //returns count of how many examples where feature = value and class = 1
  void printTree(shared_ptr<Node> node);
  string mostCommonClass(vector<vector<string>>& dataTable);
  string TreeOutput(shared_ptr<Node> node,  vector<string>& testExample);
};

#endif
