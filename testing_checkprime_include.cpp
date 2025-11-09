#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm> // For find() function

using namespace std;

// Function to check if an implicant should be added to primes
// Only returns false if the candidate is a subset of an existing prime
bool checkprime_include(const vector<int>& candidate, const vector<vector<int>>& primes) {
    // No longer check for equality - we want to include duplicates
    
    // Check if the candidate is a subset of any existing prime
    for (const auto& prime : primes) {
        // If prime contains all elements in candidate, candidate is a subset
        if (candidate.size() < prime.size()) {  // Note: changed <= to < to allow equal sets
            bool isSubset = true;
            for (int val : candidate) {
                if (find(prime.begin(), prime.end(), val) == prime.end()) {
                    isSubset = false;
                    break;
                }
            }
            if (isSubset) {
                return false; // Candidate is a proper subset of an existing prime
            }
        }
    }
    
    // Check if the candidate is a superset of any existing prime
    for (const auto& prime : primes) {
        // If candidate contains all elements in prime, prime is a subset
        if (prime.size() < candidate.size()) {  // Note: changed <= to < to allow equal sets
            bool isSuperset = true;
            for (int val : prime) {
                if (find(candidate.begin(), candidate.end(), val) == candidate.end()) {
                    isSuperset = false;
                    break;
                }
            }
            if (isSuperset) {
                return false; // Candidate is a proper superset of an existing prime
            }
        }
    }
    
    return true; // Should add to primes
}

// Main function to test the shouldAddToPrimes function
int main() {
    // Define existing primes for testing
    vector<vector<int>> primes = {{1,3}};
    
    // Test cases
    vector<vector<int>> candidates = {
        {7,15},
        {9},
        {9,8},
        {1,3},
        {1}
              // Should return true (new prime)
              
    };
    
    // Test each candidate
    for (const auto& candidate : candidates) {
        cout << "Candidate {";
        for (size_t i = 0; i < candidate.size(); ++i) {
            cout << candidate[i];
            if (i < candidate.size() - 1) cout << ",";
        }
        cout << "}: " << (checkprime_include(candidate, primes) ? "Should add" : "Should NOT add") << endl;
    }
    
    return 0;
}