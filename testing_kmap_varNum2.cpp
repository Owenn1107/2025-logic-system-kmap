#include <vector>
#include <string>
#include <unordered_set>
#include <iostream>
#include <algorithm>

using namespace std;


int rows, cols;
int greycode[4][4];
int edgecase=0;


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

bool checkessential(const vector<vector<int>>& essentials) {//true, the last one should be deleted
    vector<int> lastImplicant = essentials.back();
    for (int i = 0; i < essentials.size() - 1; i++) {
        if (lastImplicant.size() == essentials[i].size()) {
            bool same = true;
            for (size_t j = 0; j < lastImplicant.size(); j++) {
                if (lastImplicant[j] != essentials[i][j]) {
                    same = false;
                    break;
                }
            }
            if (same) return true;
        }
    }
    return false;
}

vector<vector<int>> findMinimumSOP(const vector<vector<int>>& primes, const vector<vector<int>>& essentials, const vector<int>& minterms) {
    vector<vector<int>> result;
    unordered_set<int> covered;
    unordered_set<int> allMinterms(minterms.begin(), minterms.end());

    // Step 1: Include primes that cover essential minterms
    for (const auto& prime : primes) {
        for (int m : prime) {
            // Check if m is in any of the essentials vectors
            bool isInEssentials = false;
            for (const auto& essential : essentials) {
                if (find(essential.begin(), essential.end(), m) != essential.end()) {
                    isInEssentials = true;
                    break;
                }
            }

            if (isInEssentials) {
                result.push_back(prime);
                for (int x : prime)
                    covered.insert(x);
                break;
            }
        }
    }

    // Step 2: Greedy algorithm to cover remaining minterms
    while (covered.size() < allMinterms.size()) {
        int bestIndex = -1;
        int bestCover = 0;

        for (int i = 0; i < primes.size(); ++i) {
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
        for (int x : primes[bestIndex])
            covered.insert(x);
    }

    return result;
}

void kmap_varNum2(char** kmap, KmapSolution* sol, const vector<int>& minterms){

    vector<vector<int>> essentials_2;
    vector<vector<int>> primes_2;
    vector<vector<int>> minimumSop_2;
    //edge case, all the variables in kmap are 'l' or 'x'
    if ((kmap[0][0] == '1' || kmap[0][0] == 'x') && 
        (kmap[0][1] == '1' || kmap[0][1] == 'x') && 
        (kmap[1][0] == '1' || kmap[1][0] == 'x') && 
        (kmap[1][1] == '1' || kmap[1][1] == 'x')) {
        edgecase = 1; // all 1
        sol->essentials.push_back("1");
    }
    for(int i=0; i<rows; i++){
        for(int j=0; j<cols; j++){
            int used=0;
             
            if(kmap[i][j]=='1'){
                //circle 1x2 cases
                if(kmap[i][(j+1)%2]=='1' || kmap[i][(j+1)%2]=='x'){
                    //push back to essentials with greycode[i][j] and greycode[i][(j+1)%2]
                    vector<int> candidate = {greycode[i][j], greycode[i][(j+1)%2]};
                    essentials_2.push_back( candidate);
                    used++;
                    //push back to primes (if this implicant is not already in primes or included by existing primes)
                    if(checkprime(candidate, primes_2)){
                        primes_2.push_back(candidate);
                    }
                    
                }
                //circle 2x1 cases
                if(kmap[(i+1)%2][j]=='1' || kmap[(i+1)%2][j]=='x'){
                    //push back to essentials with greycode[i][j] and greycode[(i+1)%2][j]
                    vector<int> candidate = {greycode[i][j], greycode[(i+1)%2][j]};
                    essentials_2.push_back(candidate);
                    used++;
                    //push back to primes (if this implicant is not already in primes or included by existing primes)
                    if(checkprime(candidate, primes_2)){
                        primes_2.push_back(candidate);
                    }
                }
                //deal with essentials and single 1x1 case
                if (used > 1|| checkessential(essentials_2)) {
                   essentials_2.pop_back(); // remove the last added essential implicant
                    }
                else if (used==0){
                    vector<int> candidate = {greycode[i][j]};
                    essentials_2.push_back(candidate);
                    primes_2.push_back(candidate);
                }

            }
        }
    }
    minimumSop_2 = findMinimumSOP(primes_2,essentials_2, minterms);




    cout << "primes_2:" << endl;
    for (const auto& prime :primes_2) {
        cout << "{ ";
        for (int m : prime) {
            cout << m << " ";
        }
        cout << "}" << endl;
    }
    cout << "\n'" << endl;
    cout << "essentials_2:" << endl;
    for (const auto& prime :essentials_2) {
        cout << "{ ";
        for (int m : prime) {
            cout << m << " ";
        }
        cout << "}" << endl;
    }       
    cout << "\n'" << endl;
    cout << "minimumSop_2:" << endl;
    for (const auto& prime :minimumSop_2) {
        cout << "{ ";
        for (int m : prime) {
            cout << m << " ";
        }
        cout << "}" << endl;
    }

};

int main() {
    int numVar = 2;
    vector<int> minterms = {0,1};
    vector<int> dontcares = {2,3};
    
    char** kmap = allocatekmap(numVar);
    fillkmap(kmap, numVar, minterms, dontcares);
    
   
    
    kmap_varNum2(kmap, sol, minterms);
    
    // Clean up allocated memory
    for (int i = 0; i < rows; i++) {
        delete[] kmap[i];
    }
    delete[] kmap;
    
    return 0;
}