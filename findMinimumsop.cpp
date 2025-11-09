#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>

using namespace std;

vector<vector<int>> findMinimumSOP(const vector<vector<int>>& primes, const vector<vector<int>>& essentials, const vector<int>& minterms) {
    vector<vector<int>> result;
    unordered_set<int> covered;
    unordered_set<int> allMinterms(minterms.begin(), minterms.end());

    // Step 1: Include all essential implicants directly
    for (const auto& essential : essentials) {
        // Add each essential implicant to the result
        bool isAlreadyIncluded = false;
        
        // Check if this essential is already included in result
        for (const auto& r : result) {
            if (r == essential) {
                isAlreadyIncluded = true;
                break;
            }
        }
        
        if (!isAlreadyIncluded) {
            result.push_back(essential);
            
            // Mark minterms as covered
            for (int x : essential) {
                if (allMinterms.count(x)) {
                    covered.insert(x);
                }
            }
        }
    }

    // Step 2: Greedy algorithm to cover remaining minterms
    while (covered.size() < allMinterms.size()) {
        int bestIndex = -1;
        int bestCover = 0;

        for (int i = 0; i < primes.size(); ++i) {
            // Skip if this prime is already in result
            bool alreadyIncluded = false;
            for (const auto& r : result) {
                if (r == primes[i]) {
                    alreadyIncluded = true;
                    break;
                }
            }
            if (alreadyIncluded) continue;

            // Count how many uncovered minterms this prime would cover
            int count = 0;
            for (int m : primes[i]) {
                if (allMinterms.count(m) && !covered.count(m))
                    count++;
            }

            if (count > bestCover) {
                bestCover = count;
                bestIndex = i;
            }
        }

        if (bestIndex == -1)
            break;

        result.push_back(primes[bestIndex]);
        for (int x : primes[bestIndex]) {
            if (allMinterms.count(x)) {
                covered.insert(x);
            }
        }
    }

    return result;
}

int main() {
    vector<vector<int>> primes = {
        {8,9},{13,15},{7,6,15,14}


    };

    vector<vector<int>> essentials = {{8,9},{13,15}}; // essential prime is {13,15}
    vector<int> minterms =  {8,9,13,15,7,6,14}; // needs to cover these

    vector<vector<int>> minsop = findMinimumSOP(primes, essentials, minterms);

    // 印出結果
    cout << "Minimum SOP:" << endl;
    for (const auto& prime : minsop) {
        cout << "{ ";
        for (int m : prime) {
            cout << m << " ";
        }
        cout << "}" << endl;
    }

    return 0;
}
