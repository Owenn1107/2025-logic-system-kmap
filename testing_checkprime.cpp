#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm> // For find() function

using namespace std;

// Function to check if an implicant should be added to primes
bool checkprime(const vector<int>& candidate, const vector<vector<int>>& primes) {
    // 1. Check if the candidate is equal to any existing prime
    for (const auto& prime : primes) {
        // If the candidate is identical to an existing prime, return false
        if (candidate == prime) {
            return false;
        }
    }
    
    // 2. Check if the candidate is a superset of any existing prime
    for (const auto& prime : primes) {
        // If candidate contains all elements in prime, prime is a subset
        if (prime.size() <= candidate.size()) {
            bool isSuperset = true;
            for (int val : prime) {
                if (find(candidate.begin(), candidate.end(), val) == candidate.end()) {
                    isSuperset = false;
                    break;
                }
            }
            if (isSuperset) {
                return false; // Candidate is a superset of an existing prime
            }
        }
    }
    
    // 3. Check if the candidate is a subset of any existing prime
    for (const auto& prime : primes) {
        // If prime contains all elements in candidate, candidate is a subset
        if (candidate.size() <= prime.size()) {
            bool isSubset = true;
            for (int val : candidate) {
                if (find(prime.begin(), prime.end(), val) == prime.end()) {
                    isSubset = false;
                    break;
                }
            }
            if (isSubset) {
                return false; // Candidate is a subset of an existing prime
            }
        }
    }
    
    return true; // Should add to primes
}

// Main function to test the shouldAddToPrimes function
int main() {
    // Define existing primes for testing
    vector<vector<int>> primes = {{12,9,13,8}};
    
    // Test cases
    vector<vector<int>> candidates = {
        {7,15},
        {9},
        {9,8},
        {1,3,5,7},
        {12,13,9,8}
              // Should return true (new prime)
              
    };
    
    // Test each candidate
    for (const auto& candidate : candidates) {
        cout << "Candidate {";
        for (size_t i = 0; i < candidate.size(); ++i) {
            cout << candidate[i];
            if (i < candidate.size() - 1) cout << ",";
        }
        cout << "}: " << (checkprime(candidate, primes) ? "Should add" : "Should NOT add") << endl;
    }
    
    return 0;
}