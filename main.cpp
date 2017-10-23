#include "DecisionTree.h"
#include <string>


using namespace std;



int main(int argc, char *argv[]) {
    
	string inputFile = "/Users/sarawiltberger/Desktop/Part1/data.txt";
    if (argc == 2){
        inputFile = argv[1];
    }
    
    cout << "Unbounded decision tree: " << endl;
    DecisionTree dt(inputFile, INT_MAX, false);
    //dt.printTree();
    cout<<"Train Accuracy: " << dt.getTrainAccuracy()<<endl;
    cout<<"Test Accuracy: " << dt.getTestAccuracy()<<endl;

    cout<<std::setw(6)<<"depth";
    cout<<std::setw(10)<<"train%";
    cout<<std::setw(10)<<"valid%";
    cout<<endl;
    double maxAccuracy = 0.0;
    int maxDepth = 0;
    for(int i = 1; i <= 15; i++){
        DecisionTree dt(inputFile, i);
        cout<<std::setw(6)<<i;
        cout<<std::setw(10)<<dt.getTrainAccuracy();
        cout<<std::setw(10)<<dt.getTestAccuracy();
        cout<<endl;
        if(maxAccuracy < dt.getTrainAccuracy()){
            maxAccuracy = dt.getTrainAccuracy();
            maxDepth = i;
        }
    }
    cout << endl << "Most accuate depth: " << maxDepth <<endl;
    DecisionTree dt2(inputFile, maxDepth, false);
    cout << "Accuracy: " << dt2.getTestAccuracy() <<endl;
    //buid a new tree at this depth
 
    return 0;
	
}
