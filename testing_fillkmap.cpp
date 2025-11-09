#include <iostream>
#include <vector>

using namespace std;

int greycode[4][4];
int rows, cols;

char** allocatekmap(int numVar) {
    switch (numVar) {
        case 2:
            rows = 2;
            cols = 2;
            greycode[0][0] = 0;
            greycode[0][1] = 2;
            greycode[1][0] = 1;
            greycode[1][1] = 3;
            break;
        case 3:
            rows = 2;
            cols = 4;
            greycode[0][0] = 0;
            greycode[0][1] = 2;
            greycode[0][2] = 6;
            greycode[0][3] = 4;
            greycode[1][0] = 1;
            greycode[1][1] = 3;
            greycode[1][2] = 7;
            greycode[1][3] = 5;
            break;
        case 4:
            rows = 4;
            cols = 4;
            greycode[0][0] = 0;
            greycode[0][1] = 4;
            greycode[0][2] = 12;
            greycode[0][3] = 8;
            greycode[1][0] = 1;
            greycode[1][1] = 5;
            greycode[1][2] = 13;
            greycode[1][3] = 9;
            greycode[2][0] = 3;
            greycode[2][1] = 7;
            greycode[2][2] = 15;
            greycode[2][3] = 11;
            greycode[3][0] = 2;
            greycode[3][1] = 6;
            greycode[3][2] = 14;
            greycode[3][3] = 10;
            break;
    }

    // Allocate memory for kmap
    char** kmap = new char*[rows];
    for (int i = 0; i < rows; i++) {
        kmap[i] = new char[cols];
    }

    // Initialize kmap with '0'
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            kmap[i][j] = '0';
        }
    }

    return kmap;
}
void fillkmap(char** kmap, int numVar, const vector<int> minterms, const vector<int> dontcares) {
    //fill in minterms
    for(int i=0; i<minterms.size(); i++){
        for(int j=0; j<rows; j++){
            for(int k=0; k<cols; k++){
                if(minterms[i]==greycode[j][k]){
                    kmap[j][k]='1';
                }
            }
        }
    }
    //fill in dontcares
    for(int i=0; i<dontcares.size(); i++){
        for(int j=0; j<rows; j++){
            for(int k=0; k<cols; k++){
                if(dontcares[i]==greycode[j][k]){
                    kmap[j][k]='x';
                }
            }
        }
    }
    
}


int main() {
    int numVar = 3; // Example: 3 variables
    vector<int> minterms = {0, 1, 3, 7}; // Example minterms
    vector<int> dontcares = {2, 5};      // Example don't cares

    // Allocate the K-map
    char** kmap = allocatekmap(numVar);

    // Fill the K-map with minterms and don't cares
    fillkmap(kmap, numVar, minterms, dontcares);

    // Print the K-map
    cout << "K-map for " << numVar << " variables:" << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << kmap[i][j] << " ";
        }
        cout << endl;
    }

    // Free the allocated memory
    for (int i = 0; i < rows; i++) {
        delete[] kmap[i];
    }
    delete[] kmap;

    return 0;
}