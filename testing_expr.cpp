#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

// Gray code mappings for numVar = 2, 3, and 4
map<int, vector<int>> greycode_2 = {
    {0, {0, 0}}, {2, {0, 1}},
    {1, {1, 0}}, {3, {1, 1}}
};

map<int, vector<int>> greycode_3 = {
    {0, {0, 0}}, {2, {0, 1}}, {6, {0, 2}}, {4, {0, 3}},
    {1, {1, 0}}, {3, {1, 1}}, {7, {1, 2}}, {5, {1, 3}}
};

map<int, vector<int>> greycode_4 = {
    {0, {0, 0}}, {4, {0, 1}}, {12, {0, 2}}, {8, {0, 3}},
    {1, {1, 0}}, {5, {1, 1}}, {13, {1, 2}}, {9, {1, 3}},
    {3, {2, 0}}, {7, {2, 1}}, {15, {2, 2}}, {11, {2, 3}},
    {2, {3, 0}}, {6, {3, 1}}, {14, {3, 2}}, {10, {3, 3}}
};
// Function to combine minterms into a minimized Boolean expression
string combineMintermsToExpr(int numVar, const vector<int>& group) {
    if (group.empty()) return "";
    
    // Helper function to convert an integer to its binary representation
    auto toBinary = [](int num, int bits) -> string {
        string result = "";
        for (int i = 0; i < bits; i++) {
            result = ((num & 1) ? "1" : "0") + result;
            num >>= 1;
        }
        return result;
    };
    
    // Special handling for numVar = 2
    if (numVar == 2) {
        // Get the appropriate Gray code mapping
        map<int, vector<int>> greycode = greycode_2;
        
        // Convert minterms to their Gray code positions
        vector<vector<int>> positions;
        for (int minterm : group) {
            if (greycode.find(minterm) != greycode.end()) {
                positions.push_back(greycode[minterm]);
            }
        }
        
        if (positions.empty()) return "";
        
        // Track which variables have the same value across all minterms
        vector<bool> sameValue(2, true); // Assume all bits are the same initially
        vector<bool> valueIs1(2, false); // Track if the common value is 1
        
        // For the first position, record its bit values
        for (int i = 0; i < positions[0].size(); i++) {
            valueIs1[i] = (positions[0][i] == 1);
        }
        
        // Check if bits remain the same across all positions
        for (size_t i = 1; i < positions.size(); i++) {
            for (size_t j = 0; j < positions[i].size(); j++) {
                if ((positions[i][j] == 1) != valueIs1[j]) {
                    sameValue[j] = false; // This bit varies
                }
            }
        }
        
        // Generate the Boolean expression
        string result = "";
        string vars = "ba"; // For numVar = 2, a is MSB, b is LSB
        
        for (int i = 0; i < 2; i++) {
            if (sameValue[i]) {
                // Add the variable to the expression
                result += vars[i];
                
                // Add negation if the common value is 0
                if (!valueIs1[i]) {
                    result += '\'';
                }
            }
        }
        
        return result;
    }
    
    // For numVar = 3 and numVar = 4, directly compare binary representations
    vector<string> binaries;
    for (int minterm : group) {
        binaries.push_back(toBinary(minterm, numVar));
    }
    
    if (binaries.empty()) return "";
    
    // Check which bits are the same across all minterms
    vector<bool> sameValue(numVar, true);
    vector<bool> valueIs1(numVar, false);
    
    // Set initial values from the first minterm
    for (int i = 0; i < numVar; i++) {
        valueIs1[i] = (binaries[0][i] == '1');
    }
    
    // Check if bits remain the same across all minterms
    for (size_t i = 1; i < binaries.size(); i++) {
        for (int j = 0; j < numVar; j++) {
            if ((binaries[i][j] == '1') != valueIs1[j]) {
                sameValue[j] = false; // This bit varies
            }
        }
    }
    
    // Generate the Boolean expression
    string result = "";
    string vars = (numVar == 3) ? "abc" : "abcd";
    
    for (int i = 0; i < numVar; i++) {
        if (sameValue[i]) {
            // Add the variable to the expression
            result += vars[i];
            
            // Add negation if the common value is 0
            if (!valueIs1[i]) {
                result += '\'';
            }
        }
    }
    
    return result;
}

int main() {
    
    int numVar = 2;
    vector<int> group = {0,2,1,3};
    string expr = combineMintermsToExpr(numVar, group);
    cout << "numVar = " << numVar << " expr = " << expr << endl;


    return 0;
}