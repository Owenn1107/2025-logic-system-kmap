#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <string>

using namespace std;

// Function to remove duplicates from essentials
vector<vector<int>> deletesame_essentials(const vector<vector<int>>& essentials) {
    vector<vector<int>> result;
    
    for (const auto& current : essentials) {
        bool isDuplicate = false;
        
        // Copy and sort the current implicant for comparison
        vector<int> sortedCurrent = current;
        sort(sortedCurrent.begin(), sortedCurrent.end());
        
        // Check if this implicant is already in the result (in any order)
        for (const auto& existing : result) {
            if (sortedCurrent.size() != existing.size()) continue;
            
            // Make a sorted copy of the existing implicant
            vector<int> sortedExisting = existing;
            sort(sortedExisting.begin(), sortedExisting.end());
            
            // Compare the sorted vectors
            if (sortedCurrent == sortedExisting) {
                isDuplicate = true;
                break;
            }
        }
        
        // If not a duplicate, add to result
        if (!isDuplicate) {
            result.push_back(current);
        }
    }
    
    return result;
}

// Main function for testing
int main() {
    // Test case from the example
    vector<vector<int>> essentials = {
      {2,4,3,1},
      {2,4,3,1},
      {2,4,1,3},
      {4,2,3,1},
      {2,1},
      {1,3}
    };
    
    cout << "Original essentials: " << endl;
    for (const auto& imp : essentials) {
        cout << "{ ";
        for (int val : imp) {
            cout << val << " ";
        }
        cout << "}" << endl;
    }
    
    vector<vector<int>> result = deletesame_essentials(essentials);
    
    cout << "\nAfter removing duplicates: " << endl;
    for (const auto& imp : result) {
        cout << "{ ";
        for (int val : imp) {
            cout << val << " ";
        }
        cout << "}" << endl;
    }
    
    return 0;
}